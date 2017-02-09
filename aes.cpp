#include "main.h"
#include <openssl/evp.h>

namespace Main {

    namespace Crypt {

        bool encryptAES(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen){
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

            *dst = (unsigned char*) malloc(*dstLen = dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), *dstLen);
            return true;
        }

        bool decryptAES(unsigned char* text, int textLen, unsigned char key[32], unsigned char* iv, int BUFSIZE, unsigned char** dst, int* dstLen){
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

            *dst = (unsigned char*) malloc(*dstLen = dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), *dstLen);
            return true;
        }

    }

}
