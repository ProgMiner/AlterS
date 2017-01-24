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
#include <openssl/evp.h>

#define APPNAME ("AlterS")
#define APPVERSION ("v0.1")
#define SETTINGSFILE (QDir::homePath() + "/.AlterS/settings.xml")
#define ACCOUNTSDIR (QDir::homePath() + "/.AlterS/accounts/")
#define DEFAULTCODECNAME ("UTF-8")
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER (QTextDecoder(&DEFAULTCODEC))

namespace Main {

    struct AES{

        //SHA256 hash for generate 32-byte array from any-length password
        static void sha256(unsigned char* text, int textLen, unsigned char dst[32]){
            SHA256(text, textLen, dst);
        }

        static bool encrypt(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen){
            unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
            int inlen, outlen, offset = 0;
            EVP_CIPHER_CTX ctx;
            QByteArray dstArr;

            EVP_CIPHER_CTX_init(&ctx);
            EVP_EncryptInit(&ctx, EVP_aes_256_ofb(), key, iv);

            while(true){
                inlen = textLen - offset;
                inlen = inlen < BUFSIZE ? inlen : BUFSIZE;
                if(inlen < 1) break;
                memcpy(inbuf, text + offset, inlen);
                if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return false;
                dstArr.append((char*) outbuf, outlen);
                offset += inlen;
            }

            if(!EVP_EncryptFinal(&ctx, outbuf, &outlen)) return false;
            dstArr.append((char*) outbuf, outlen);
            EVP_CIPHER_CTX_cleanup(&ctx);

            *dst = (unsigned char*) malloc(dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), dstArr.size());
            *dstLen = dstArr.size();
            return true;
        }

        static bool decrypt(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen){
            unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
            int inlen, outlen, offset = 0;
            EVP_CIPHER_CTX ctx;
            QByteArray dstArr;

            EVP_CIPHER_CTX_init(&ctx);
            EVP_DecryptInit(&ctx, EVP_aes_256_ofb(), key, iv);

            while(true){
                inlen = textLen - offset;
                inlen = inlen < BUFSIZE ? inlen : BUFSIZE;
                if(inlen < 1) break;
                memcpy(inbuf, text + offset, inlen);
                if(!EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return false;
                dstArr.append((char*) outbuf, outlen);
                offset += inlen;
            }

            if(!EVP_DecryptFinal(&ctx, outbuf, &outlen)) return false;
            dstArr.append((char*) outbuf, outlen);
            EVP_CIPHER_CTX_cleanup(&ctx);

            *dst = (unsigned char*) malloc(dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), dstArr.size());
            *dstLen = dstArr.size();
            return true;
        }

    };

    struct Account;

    extern Account currentAccount;
    extern QList<Account> contacts;
    extern MainWindow* mainWindow;

    extern QList<QString> accountList();

}
