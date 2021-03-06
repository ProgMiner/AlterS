#pragma once

#include "main.h"
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <QString>

namespace Main{

    struct Account{

        RSA* key;
        QString id, nickname, statusText;
        QPixmap* avatar = nullptr;

        QString showingName(int maxSize = 0){
            QString ret = nickname.isEmpty() ? id : nickname;
            if(maxSize == 0 || ret.size() <= maxSize) return ret;
            return ret.left(maxSize - 3) + "...";
        }

        int compileData(unsigned char** dst){
            QByteArray ret;
            //QString id
            ret.append(DEFAULTCODEC.fromUnicode(id)).append('\0');
            { //RSA* key
                BIO* bio = BIO_new(BIO_s_mem());
                PEM_write_bio_RSAPrivateKey(bio, key, nullptr, nullptr, 0, nullptr, nullptr);
                unsigned char* pem = (unsigned char*) malloc(10240);
                int len = BIO_read(bio, pem, 10240);
                BIO_free(bio);
                pem = (unsigned char*) realloc(pem, len + 1);
                pem[len] = (unsigned char) '\0';
                ret.append(QByteArray::fromRawData((char*) pem, len + 1));
                free(pem);
            } //QString nickname
            ret.append(DEFAULTCODEC.fromUnicode(nickname)).append('\0');
            //QString statusText
            ret.append(DEFAULTCODEC.fromUnicode(statusText)).append('\0');
            { //QPixmap avatar
                QByteArray ba;
                QBuffer buf(&ba);
                if(buf.open(QIODevice::WriteOnly)){
                    avatar->save(&buf, "PNG");
                    buf.close();
                }
                ret.append(qCompress(ba, 9).toBase64()).append('\0');
            }

            *dst = (unsigned char*) malloc(ret.size());
            memcpy(*dst, ret.data(), ret.size());
            return ret.size();
        }

        bool save(unsigned char key[32], unsigned char** dst, int* dstLen){
            unsigned char *tmp1, *tmp2;
            int tmpi1, tmpi2;
            //Get compiled data
            tmpi1 = compileData(&tmp1);
            //Get compiled data's hash and make data to encrypt
            tmp2 = (unsigned char*) malloc(tmpi2 = 32 + tmpi1);
            Crypt::sha256(tmp1, tmpi1, tmp2);
            memcpy(tmp2 + 32, tmp1, tmpi1);
            free(tmp1);
            { //Encrypt
                unsigned char iv[1024];
                RAND_bytes(iv, 1024);
                if(!Crypt::encryptAES(tmp2, tmpi2, key, iv, 1024, &tmp1, &tmpi1)) return false;
                free(tmp2);
                //Make data to return
                tmp2 = (unsigned char*) malloc(tmpi2 = tmpi1 + 1024);
                memcpy(tmp2, iv, 1024);
                memcpy(tmp2 + 1024, tmp1, tmpi1);
                free(tmp1);
            } //Return
            *dst = (unsigned char*) malloc(tmpi2);
            memcpy(*dst, tmp2, *dstLen = tmpi2);
            free(tmp2);
            return true;
        }

        bool saveFile(unsigned char key[32], QString filename){
            QFile file(filename);
            if(!file.open(QIODevice::WriteOnly)) return false;
            unsigned char* src;
            int srcLen = 0;
            if(!save(key, &src, &srcLen)) return false;
            file.write((char*) src, srcLen);
            file.close();
            return true;
        }

        static Account decompileData(unsigned char* src, int srcLen){
            QList<QByteArray> srcList = QByteArray::fromRawData((char*) src, srcLen).split('\0');
            Account ret;
            //QString id
            ret.id = DEFAULTCODEC.toUnicode(srcList[0].data(), srcList[0].size());
            { //RSA* key
                BIO* bio = BIO_new(BIO_s_mem());
                BIO_write(bio, srcList[1].data(), srcList[1].size());
                ret.key = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
                BIO_free(bio);
            } //QString nickname
            ret.nickname = DEFAULTCODEC.toUnicode(srcList[2].data(), srcList[2].size());
            //QString statusText
            ret.statusText = DEFAULTCODEC.toUnicode(srcList[3].data(), srcList[3].size());
            {//QPixmap
                srcList[4] = qUncompress(QByteArray::fromBase64(srcList[4]));
                ret.avatar = new QPixmap();
                ret.avatar->loadFromData(srcList[4], "PNG");
            }

            return ret;
        }

        static bool load(unsigned char* src, int srcLen, unsigned char key[32], Account* dst){
            unsigned char *tmp1, *tmp2;
            int tmpi1, tmpi2;
            { //Parse data
                unsigned char iv[1024];
                memcpy(iv, src, 1024);
                tmp1 = (unsigned char*) malloc(tmpi1 = srcLen - 1024);
                memcpy(tmp1, src + 1024, tmpi1);
                //Decrypt
                if(!Crypt::decryptAES(tmp1, tmpi1, key, iv, 1024, &tmp2, &tmpi2)) return false;
                free(tmp1);
            } //Parse decrypted data and test
            tmp1 = (unsigned char*) malloc(tmpi1 = tmpi2 - 32);
            memcpy(tmp1, tmp2 + 32, tmpi1);
            realloc(tmp2, 32);
            {
                unsigned char sha[32];
                Crypt::sha256(tmp1, tmpi1, sha);
                if(memcmp(sha, tmp2, 32) != 0) return false;
                free(tmp2);
            }//Parse compiled data
            *dst = decompileData(tmp1, tmpi1);
            return true;
        }

        static bool loadFile(unsigned char key[32], QString filename, Account* dst){
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly)) return false;
            unsigned char* src;
            int srcLen = 0;
            {
                QByteArray ba = file.readAll();
                src = (unsigned char*) malloc(srcLen = ba.size());
                memcpy(src, ba.data(), srcLen);
            }
            file.close();
            if(!load(src, srcLen, key, dst)) return false;
            free(src);
            return true;
        }

        static QString generateID(RSA* rsa){
            BIO* bio = BIO_new(BIO_s_mem());
            PEM_write_bio_RSAPublicKey(bio, rsa);
            unsigned char* pem = (unsigned char*) malloc(10240);
            int len = BIO_read(bio, pem, 10240);
            BIO_free(bio);
            pem = (unsigned char*) realloc(pem, len);
            unsigned char sha[32];
            SHA256(pem, len, sha);
            free(pem);
            return QString(QByteArray::fromRawData((char*) sha, 32).toHex());
        }

        static Account create(void* seedBuf, int seedLen){
            RAND_seed(seedBuf, seedLen);
            RSA* rsa = RSA_new();
            BIGNUM* e = BN_new();
            BN_dec2bn(&e, "65537");
            RSA_generate_key_ex(rsa, 2048, e, NULL);
            BN_free(e);
            Account ret;
            ret.key = rsa;
            ret.id = generateID(rsa);
            ret.avatar = new QPixmap;
            ret.avatar->load(":image/avatar.png", "PNG");
            return ret;
        }

    };

}
