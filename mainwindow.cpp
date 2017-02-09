#include "mainwindow.h"
#include "mainpagewidget.h"
#include "accountpagewidget.h"
#include "settingspagewidget.h"
#include "ui_mainwindow.h"
#include "account.h"
#include <QDesktopServices>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setWindowTitle((QString) APPNAME + ": " + Main::currentAccount.showingName());
    QPalette pal = palette();
    pal.setBrush(QPalette::Background, QBrush(Qt::white));
    setPalette(pal);
    setCentralWidget(new MainPageWidget(this));
    applyChanges();
}

void MainWindow::applyChanges(){
    //Contacts
    QActionGroup* gr = nullptr;
    if(ui->menu->actions().size() > 0) gr = ui->menu->actions()[0]->actionGroup();
    else gr = new QActionGroup(this);
    ui->menu->clear();
    if(gr->actions().size() == 0){
        if(Main::contacts.size() != 0)
            for(Main::Account acc : Main::contacts)
                gr->addAction(ui->menu->addAction(acc.showingName()));
        else ui->menu->addAction("Нет контактов")->setEnabled(false);
    }
    connect(gr, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_triggered(QAction*)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::actionGroup_triggered(QAction* action){
    //
}

void MainWindow::on_action_4_triggered(){
    QApplication::exit();
}

void MainWindow::on_action_2_triggered(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("О программе");
    msgBox.setWindowIcon(windowIcon());
    msgBox.setText((QString) APPNAME + " " + (QString) APPVERSION + " by ProgMiner");
    msgBox.addButton("OK", QMessageBox::NoRole);
    msgBox.addButton("О Qt", QMessageBox::HelpRole);
    msgBox.addButton("by ProgMiner", QMessageBox::YesRole);
    msgBox.setDefaultButton(QMessageBox::No);
    //msgBox.setIconPixmap(QPixmap(":res/icon.png").scaled(64, 64));
    int i = msgBox.exec();
    if(i == 1) QMessageBox::aboutQt(this, "О Qt");
    if(i == 2) if(QMessageBox::question(this, "Подтверждение", "Открыть ссылку в браузере?", "Да", "Нет") == 0)
        QDesktopServices::openUrl(QUrl("http://byprogminer.esy.es/"));
}

void MainWindow::on_action_triggered(){
    setCentralWidget(new MainPageWidget(this));
}

void MainWindow::on_action_5_triggered(){
    setCentralWidget(new AccountPageWidget(Main::currentAccount, this));
}

void MainWindow::on_action_7_triggered(){
    setCentralWidget(new SettingsPageWidget(this));
}
