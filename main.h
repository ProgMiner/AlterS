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
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <iostream>

#define APPNAME ("AlterS")
#define APPVERSION ("v0.1")
#define SETTINGSFILE (QDir::homePath() + "/.AlterS/settings.xml")
#define ACCOUNTSDIR (QDir::homePath() + "/.AlterS/accounts/")
#define DEFAULTCODECNAME ("UTF-8")
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER (QTextDecoder(&DEFAULTCODEC))

namespace Main {

    struct AES{

        static unsigned char* encrypt(unsigned char* text, int textLen, unsigned char key[32]){

        }

    };

    struct Account{

        RSA* key;
        QString nickname;

        static Account create(void* seedBuf, int seedNum){
            RAND_seed(seedBuf, seedNum);
            RSA* rsa = RSA_new();
            BIGNUM* e = BN_new();
            BN_dec2bn(&e, "65537");
            RSA_generate_key_ex(rsa, 2048, e, NULL);
            Account ret;
            ret.key = rsa;
            return ret;
        }

    };

    extern Account currentAccount;

}
#endif // MAIN_H
/*
  {
      BIO* bio = BIO_new(BIO_s_mem());
      PEM_write_bio_RSAPublicKey(bio, key);
      char pem[2048];
      BIO_read(bio, pem, 2048);
      BIO_free(bio);
      unsigned char sha[64];
      SHA512((unsigned char*) pem, 2048, sha);
      free(pem);

  }
*/
