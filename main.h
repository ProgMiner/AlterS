#ifndef MAIN_H
#define MAIN_H

#include "mainwindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtXml>
#include <QFile>
#include <QDir>
#include <openssl/rsa.h>

#define APPNAME ("AlterS")
#define APPVERSION ("v0.1")
#define SETTINGSFILE (QDir::homePath() + "/.AlterS/settings.xml")
#define ACCOUNTSDIR (QDir::homePath() + "/.AlterS/accounts/")
#define DEFAULTCODECNAME ("UTF-8")
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER (QTextDecoder(&DEFAULTCODEC))

namespace Main {

    struct Account;

    extern Account currentAccount;

}
#endif // MAIN_H
