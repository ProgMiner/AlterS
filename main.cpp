#include "main.h"
#include "mainwindow.h"
#include "account.h"

namespace Main{

    Account nowAccount;

}

using namespace Main;

QList<QString> accountList(){
    QDir accDir(ACCOUNTSDIR);
    if(!accDir.exists()) accDir.mkpath(ACCOUNTSDIR);
    QStringList lst = accDir.entryList();
    QList<QString> ret;
    foreach (QString s, lst)
        if(s.endsWith(".account")) ret.push_back(s);
    return ret;
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    if(accountList().size() < 1){
        if(QMessageBox::question(nullptr, APPNAME, "Нет учётных записей. Создать?", "Да", "Нет") == 1) return 1;
        QString seedString = QInputDialog::getText(nullptr, APPNAME, "Введите случайные символы для увеличения случайности ключа",
                                                   QLineEdit::Normal, QString(), nullptr,
                                                   QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
        Account acc = Account::create(seedString.toUtf8().data(), seedString.toUtf8().size());
        acc.nickname = "ProgMiner";
        Account load;
        unsigned char* save;
        int saveLen;
        qDebug() << acc.save((unsigned char*) "testkey", &save, &saveLen);
        qDebug() << Account::load(save, saveLen, (unsigned char*) "testkey", &load);
        qDebug() << load.nickname;
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
