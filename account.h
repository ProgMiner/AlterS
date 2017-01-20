#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <QString>

namespace Main{

    struct Account{

        RSA* key;
        QString nickname;

        static Account create(void* seedBuf, int seedLen){
            RAND_seed(seedBuf, seedLen);
            RSA* rsa = RSA_new();
            BIGNUM* e = BN_new();
            BN_dec2bn(&e, "65537");
            RSA_generate_key_ex(rsa, 2048, e, NULL);
            Account ret;
            ret.key = rsa;
            return ret;
        }

    };

}
