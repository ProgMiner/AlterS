#include "main.h"
#include "mainwindow.h"
#include <QApplication>
#include <openssl/aes.h>

namespace Main{

    QList<Account> parseFile(QByteArray file){

    }

}

using namespace Main;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QFile accFile(ACCOUNTSFILE);
    if(!accFile.exists()){
        if(QMessageBox::question(nullptr, APPNAME, "Нет учётной записи. Создать?", "Да", "Нет") == 1) return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
