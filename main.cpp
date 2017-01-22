#include "main.h"
#include "mainwindow.h"
#include "account.h"

namespace Main{

    Account currentAccount;

}

using namespace Main;

QList<QString> accountList(){
    QDir accDir(ACCOUNTSDIR);
    if(!accDir.exists()) accDir.mkpath(ACCOUNTSDIR);
    QStringList lst = accDir.entryList();
    QList<QString> ret;
    foreach (QString s, lst)
        if(s.endsWith(".account")) ret.push_back(QFileInfo(s).baseName());
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
        QString password = QInputDialog::getText(nullptr, APPNAME, "Введите пароль для аккаунта\nВнимание! Восстановить пароль невозможно!",
                                                 QLineEdit::Normal, QString(), nullptr,
                                                 QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
        {
            unsigned char key[32];
            QByteArray ba = DEFAULTCODEC.fromUnicode(password);
            AES::sha256((unsigned char*) ba.data(), ba.size(), key);
            acc.saveFile(key, ACCOUNTSDIR + acc.id + ".account");
        }
        currentAccount = acc;
    } else {
        selectacc:
        QList<QString> accList = accountList();
        bool ok = false;
        QString accName = QInputDialog::getItem(nullptr, APPNAME, "Обнаружены следующие аккаунты.\nВыберите аккаунт для входа:",
                                                QStringList(accList), 0, false, &ok,
                                                QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
        if(!ok) return 0;
        bool ret = false;
        QString password = QInputDialog::getText(nullptr, APPNAME, "Введите пароль от аккаунта \"" + accName + "\"",
                                                 QLineEdit::Normal, QString(), &ok,
                                                 QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
        if(!ok) goto selectacc;
        Account acc;
        while(!ret){
            unsigned char key[32];
            QByteArray ba = DEFAULTCODEC.fromUnicode(password);
            AES::sha256((unsigned char*) ba.data(), ba.size(), key);
            ret = Account::loadFile(key, ACCOUNTSDIR + accName + ".account", &acc);
            if(!ret) password = QInputDialog::getText(nullptr, APPNAME, "Неверный пароль, попробуйте снова:",
                                                      QLineEdit::Normal, QString(), &ok,
                                                      QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint);
            if(!ok) goto selectacc;
        }
        currentAccount = acc;

    }

    MainWindow w;
    w.show();

    return a.exec();
}
