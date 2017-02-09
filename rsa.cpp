#include "main.h"
#include <openssl/pem.h>

namespace Main {

    namespace Crypt {

        bool encryptRSA(unsigned char* text, int textLen, RSA* key, unsigned char** dst, int* dstLen){
            if(!key) return false;
            const int keySize = RSA_size(key);
            unsigned char inbuf[keySize], outbuf[keySize];
            int inlen, outlen, offset = 0;
            QByteArray dstArr;

            while(true){
                inlen = textLen - offset;
                inlen = inlen < keySize ? inlen : keySize;
                if(inlen < 1) break;
                memcpy(inbuf, text + offset, inlen);
                if((outlen = RSA_public_encrypt(inlen, inbuf, outbuf, key, RSA_PKCS1_PADDING)) != keySize) return false;
                dstArr.append((char*) outbuf, outlen);
                offset += inlen;
            }

            *dst = (unsigned char*) malloc(*dstLen = dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), *dstLen);
            return true;
        }

        bool decryptRSA(unsigned char* text, int textLen, RSA* key, unsigned char** dst, int* dstLen){
            if(!key) return false;
            const int keySize = RSA_size(key);
            unsigned char inbuf[keySize], outbuf[keySize];
            int inlen, outlen, offset = 0;
            QByteArray dstArr;

            while(true){
                inlen = textLen - offset;
                inlen = inlen < keySize ? inlen : keySize;
                if(inlen < 1) break;
                memcpy(inbuf, text + offset, inlen);
                if((outlen = RSA_private_decrypt(inlen, inbuf, outbuf, key, RSA_PKCS1_PADDING)) < 0) return false;
                dstArr.append((char*) outbuf, outlen);
                offset += inlen;
            }

            *dst = (unsigned char*) malloc(*dstLen = dstArr.size());
            memcpy(*dst, (unsigned char*) dstArr.data(), *dstLen);
            return true;
        }

    }

}
