#pragma once

#include "mainwindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtXml>
#include <QFile>
#include <QDir>
#include <openssl/sha.h>
#include <openssl/pem.h>

#define APPNAME ("AlterS")
#define APPVERSION ("v0.1")
#define SETTINGSFILE (QDir::homePath() + "/.AlterS/settings.xml")
#define ACCOUNTSDIR (QDir::homePath() + "/.AlterS/accounts/")
#define DEFAULTCODECNAME ("UTF-8")
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER (QTextDecoder(&DEFAULTCODEC))

namespace Main {

    namespace Crypt{

        //SHA256 hash for generate 32-byte array from any-length password
        void sha256(unsigned char* text, int textLen, unsigned char dst[32]);
        bool encryptAES(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen);
        bool decryptAES(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen);
        bool encryptRSA(unsigned char* text, int textLen, RSA* key, unsigned char** dst, int* dstLen);
        bool decryptRSA(unsigned char* text, int textLen, RSA* key, unsigned char** dst, int* dstLen);

    }

    struct Account;

    extern Account currentAccount;
    extern QList<Account> contacts;
    extern MainWindow* mainWindow;

    extern QList<QString> accountList();

}
