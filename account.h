#include "main.h"
#include "aes.h"
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <QString>

namespace Main{

    struct Account{

        RSA* key;
        QString id, nickname;

        int compileData(unsigned char** dst){
            QByteArray ret;
            //RSA* key
            {
                BIO* bio = BIO_new(BIO_s_mem());
                PEM_write_bio_RSAPrivateKey(bio, key, nullptr, nullptr, 0, nullptr, nullptr);
                unsigned char* pem = (unsigned char*) malloc(10240);
                int len = BIO_read(bio, pem, 10240);
                BIO_free(bio);
                pem = (unsigned char*) realloc(pem, len + 1);
                pem[len] = (unsigned char) '\0';
                ret.append(QByteArray::fromRawData((char*) pem, len + 1));
                free(pem);
            }
            //QString nickname
            ret.append(nickname + "\0");

            *dst = (unsigned char*) malloc(ret.size());
            memcpy(*dst, ret.data(), ret.size());
            return ret.size();
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
            return ret;
        }

    };

}
