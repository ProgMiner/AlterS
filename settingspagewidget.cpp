#include "settingspagewidget.h"
#include "ui_settingspagewidget.h"
#include "main.h"
#include "account.h"
#include <QMenu>
#include <QFileDialog>

SettingsPageWidget::SettingsPageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SettingsPageWidget){
    ui->setupUi(this);
    //General

    //Accounts
    ui->tableWidget->setColumnWidth(0, 400);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    refreshAccountsTable();
    //Account
    ui->lineEdit->setText(Main::currentAccount.nickname);
    ui->plainTextEdit->setPlainText(Main::currentAccount.statusText);
    ui->label->setPixmap(*Main::currentAccount.avatar);
    ui->label->setMask(QRegion(-1, -1, 64, 64, QRegion::Ellipse));
}

void SettingsPageWidget::refreshAccountsTable(){
    ui->tableWidget->setRowCount(0);
    for(QString string : Main::accountList()){
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(string, 0));
    }
}

SettingsPageWidget::~SettingsPageWidget(){
    delete ui;
}

void SettingsPageWidget::toTab(int i){
    ui->tabWidget->setCurrentIndex(i);
}

void SettingsPageWidget::on_tableWidget_customContextMenuRequested(){
    QMenu* menu = new QMenu(this);
    menu->addAction(ui->action);
    menu->addAction(ui->action_3);
    if(ui->tableWidget->currentRow() > -1){
        menu->addSeparator();
        menu->addAction(ui->action_2);
    }
    menu->exec(QCursor::pos());
}

void SettingsPageWidget::on_action_triggered(){
    bool ok = false;
    QString seedString = QInputDialog::getText(nullptr, "Новый аккаунт", "Введите случайные символы для увеличения случайности ключа",
                                               QLineEdit::Normal, QString(), &ok,
                                               QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
    if(!ok) return;
    Main::Account acc = Main::Account::create(seedString.toUtf8().data(), seedString.toUtf8().size());
    QString password = QInputDialog::getText(nullptr, "Новый аккаунт", "Введите пароль для аккаунта\nВнимание! Восстановить пароль невозможно!",
                                             QLineEdit::Normal, QString(), &ok,
                                             QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
    if(!ok) return;
    {
        unsigned char key[32];
        QByteArray ba = DEFAULTCODEC.fromUnicode(password);
        Main::AES::sha256((unsigned char*) ba.data(), ba.size(), key);
        acc.saveFile(key, ACCOUNTSDIR + acc.id + ".account");
    }
    refreshAccountsTable();
}

void SettingsPageWidget::on_action_3_triggered(){
    refreshAccountsTable();
}

void SettingsPageWidget::on_action_2_triggered(){
    QFile file(ACCOUNTSDIR + ui->tableWidget->currentItem()->text() + ".account");
    if(!file.remove()) QMessageBox::warning(this, "Ошибка", "Не удалось удалить аккаунт:\n" + file.errorString(), "OK");
    refreshAccountsTable();
}

void SettingsPageWidget::on_pushButton_clicked(){
    bool ok = false;
    QString password = QInputDialog::getText(nullptr, "Сохранение аккаунта", "Введите пароль от аккаунта",
                                             QLineEdit::Normal, QString(), &ok,
                                             QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
    if(!ok) return;
    unsigned char key[32];
    QByteArray ba = DEFAULTCODEC.fromUnicode(password);
    Main::AES::sha256((unsigned char*) ba.data(), ba.size(), key);
    if(!Main::currentAccount.loadFile(key, ACCOUNTSDIR + Main::currentAccount.id + ".account", new Main::Account))
        if(QMessageBox::question(this, "Сохранение аккаунта", "Пароль не совпадает со старым или аккаунт удалён. Сохранить с новым паролем?", "Да", "Нет") == 1) return;
    Main::currentAccount.saveFile(key, ACCOUNTSDIR + Main::currentAccount.id + ".account");
}

void SettingsPageWidget::on_lineEdit_textChanged(const QString &arg1){
    Main::currentAccount.nickname = arg1;
}

void SettingsPageWidget::on_plainTextEdit_textChanged(){
    Main::currentAccount.statusText = ui->plainTextEdit->toPlainText();
}

void SettingsPageWidget::on_pushButton_3_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Смена аватара", QDir::homePath(), "Изображения PNG (*.png)");
    if(path.isEmpty()) return;
    if(!Main::currentAccount.avatar->load(path, "PNG")) QMessageBox::warning(this, "Смена аватара", "Не удалось изменить аватар.", "OK");
    *Main::currentAccount.avatar = Main::currentAccount.avatar->scaled(64, 64);
    ui->label->setPixmap(*Main::currentAccount.avatar);
}

void SettingsPageWidget::on_pushButton_4_clicked(){
    if(QMessageBox::question(this, "Удаление аватара", "Вы действительно хотите удалить установленный аватар?", "Да", "Нет") == 1) return;
    Main::currentAccount.avatar->load(":image/avatar.png", "PNG");
    ui->label->setPixmap(*Main::currentAccount.avatar);
}
