#include "mainpagewidget.h"
#include "ui_mainpagewidget.h"
#include <QDesktopServices>
#include <QUrl>

MainPageWidget::MainPageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainPageWidget){
    ui->setupUi(this);
}

MainPageWidget::~MainPageWidget(){
    delete ui;
}

void MainPageWidget::on_commandLinkButton_clicked(){
    QDesktopServices::openUrl(QUrl("http://byprogminer.esy.es/projects/AlterS"));
}
