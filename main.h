#ifndef MAIN_H
#define MAIN_H

#include "mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>

#define APPNAME "Cipherinator"
#define APPVERSION "v0.1"
#define SETTINGSFILE (QDir::homePath() + "/.AlterS/settings.xml")
#define ACCOUNTSFILE (QDir::homePath() + "/.AlterS/accounts.bin")
#define DEFAULTCODECNAME "UTF-8"
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER QTextDecoder(&DEFAULTCODEC)

namespace Main {

    struct Account{



    };

    extern QList<Account> parseFile(QByteArray file);

}
#endif // MAIN_H
