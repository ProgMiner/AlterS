#include "main.h"
#include "mainwindow.h"
#include "aes.h"
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

    unsigned char key[32], iv[8];
    unsigned char *enc = nullptr, *dec = nullptr;
    int encLen = 0, decLen = 0;
    AES::sha256((unsigned char*) "testkey", 8, key);
    RAND_bytes(iv, 8);
    qDebug() << AES::encrypt((unsigned char*) "testtext", 9, key, iv, 4, &enc, &encLen);
    qDebug() << AES::decrypt(enc, encLen, key, iv, 4, &dec, &decLen);
    qDebug() << QByteArray::fromRawData((char*) dec, decLen);
    return 0;

    if(accountList().size() < 1){
        if(QMessageBox::question(nullptr, APPNAME, "Нет учётных записей. Создать?", "Да", "Нет") == 1) return 1;
        QString seedString = QInputDialog::getText(nullptr, APPNAME, "Введите случайные символы для увеличения случайности ключа",
                                                   QLineEdit::Normal, QString(), nullptr,
                                                   QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
        Account acc = Account::create(seedString.toUtf8().data(), seedString.toUtf8().size());

        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
