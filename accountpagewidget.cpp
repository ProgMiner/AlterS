#include "accountpagewidget.h"
#include "ui_accountpagewidget.h"
#include "settingspagewidget.h"
#include "account.h"
#include <QFileDialog>
#include <QScrollArea>
#include <QMenu>

AccountPageWidget::AccountPageWidget(Main::Account acc, QWidget *parent) : QWidget(parent), ui(new Ui::AccountPageWidget){
    ui->setupUi(this);
    hide();
    this->account = acc;
    ui->label->setPixmap(*acc.avatar);
    ui->label->setMask(QRegion(-1, -1, 64, 64, QRegion::Ellipse));
    ui->label_2->setText(acc.showingName());
    ui->label_3->setText(acc.statusText);
    ui->label_5->setText(acc.id);
    show();
}

AccountPageWidget::~AccountPageWidget(){
    delete ui;
}

void AccountPageWidget::on_pushButton_clicked(){
    BIO* bio = BIO_new(BIO_s_mem());
    RSA_print(bio, account.key, 0);
    char rsa[10240];
    int rsaLen = BIO_read(bio, rsa, 10240);
    BIO_free(bio);
    QString str(QByteArray(rsa, rsaLen));
    QDialog* dlg = new QDialog(nullptr, QMessageBox().windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout* dlgLay = new QGridLayout(dlg);
    QScrollArea* scr = new QScrollArea(dlg);
    QLabel* lbl = new QLabel(str, scr);
    lbl->setMargin(5);
    scr->setWidget(lbl);
    scr->setMinimumWidth(300);
    scr->setMinimumHeight(300);
    scr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scr->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    dlgLay->addWidget(scr);
    dlgLay->setMargin(0);
    dlg->setLayout(dlgLay);
    dlg->show();
}

void AccountPageWidget::on_pushButton_2_clicked(){
    QMenu* menu = new QMenu(this);
    QAction *pubKey = menu->addAction("Открытый ключ"), *prvKey = menu->addAction("Закрытый ключ");
    connect(pubKey, SIGNAL(triggered(bool)), this, SLOT(exportPubkey_action()));
    connect(prvKey, SIGNAL(triggered(bool)), this, SLOT(exportPrvkey_action()));
    menu->exec(QCursor::pos());
}

void AccountPageWidget::exportPrvkey_action(){
    if(QMessageBox::question(this, "Экспорт", "Вы уверены? В случае получения ключа злоумышлинником Ваши данные могут быть скомпрометированны", "Да", "Нет") != 0) return;
    QString path = QFileDialog::getSaveFileName(this, "Экспорт", QDir::homePath(), "Закрытый ключ RSA (*.key);;*.*", new QString("Закрытый ключ RSA (*.key)"));
    if(path.isEmpty()) return;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Ошибка", "Не удалось экспортировать ключ\n" + file.errorString(), "OK");
        return;
    }
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, account.key, nullptr, nullptr, 0, nullptr, nullptr);
    char rsa[10240];
    int rsaLen = BIO_read(bio, rsa, 10240);
    BIO_free(bio);
    file.write(rsa, rsaLen);
    file.close();
}

void AccountPageWidget::exportPubkey_action(){
    QString path = QFileDialog::getSaveFileName(this, "Экспорт", QDir::homePath(), "Открытый ключ RSA (*.pub);;*.*", new QString("Открытый ключ RSA (*.pub)"));
    if(path.isEmpty()) return;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Ошибка", "Не удалось экспортировать ключ\n" + file.errorString(), "OK");
        return;
    }
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, account.key);
    char rsa[10240];
    int rsaLen = BIO_read(bio, rsa, 10240);
    BIO_free(bio);
    file.write(rsa, rsaLen);
    file.close();
}

void AccountPageWidget::on_commandLinkButton_clicked(){
    SettingsPageWidget* page = new SettingsPageWidget(Main::mainWindow);
    page->toTab(2);
    Main::mainWindow->setCentralWidget(page);
}
