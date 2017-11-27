#include <stdio.h>
#include <string.h>
#include <time.h>

#include "rand.h"
#include "fips.h"

#include <openssl/evp.h>
#include <openssl/err.h>

const size_t DIGEST_LENGTH = 40;

unsigned int calc_hash(const char* mode, const char* in, size_t size, unsigned char* out) {
    unsigned int md_len = -1;
    const EVP_MD *md = EVP_get_digestbyname(mode);
    if (NULL != md) {
        EVP_MD_CTX mdctx;
        EVP_MD_CTX_init(&mdctx);
        EVP_DigestInit_ex(&mdctx, md, NULL);
        EVP_DigestUpdate(&mdctx, in, size);
        EVP_DigestFinal_ex(&mdctx, out, &md_len);
        EVP_MD_CTX_cleanup(&mdctx);
    }
    return md_len;
}
typedef enum { false, true } bool;

int main(int argc, char *argv[]) {
    if (FIPS_init(1) == 1) {
        unsigned long err_code = ERR_get_error();
        if (err_code == 0) {
            return 1;
        }
        const size_t ERR_BUFFER_SIZE = 120;
        char *err_buf = (char*)malloc(sizeof(char) * ERR_BUFFER_SIZE);
        ERR_error_string(err_code, err_buf);

        printf("error while initializing FIPS mode: %s", err_buf);
        return 1;
    }

    char *alg = "SHA256";
    bool use_rand = false;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-sha1") == 0) {
            alg = "SHA1";
            continue;
        }
        if (strcmp(argv[i], "-sha224") == 0) {
            alg = "SHA224";
            continue;
        }
        if (strcmp(argv[i], "-sha256") == 0) {
            alg = "SHA256";
            continue;
        }
        if (strcmp(argv[i], "-sha384") == 0) {
            alg = "SHA384";
            continue;
        }
        if (strcmp(argv[i], "-sha512") == 0) {
            alg = "SHA512";
            continue;
        }

        if (strcmp(argv[i], "-use-rand") == 0) {
            use_rand = true;
            continue;
        }
    }

    char *in;
    if (use_rand == true) {
        const size_t BUFFER_SIZE = 40;
        srand(time(NULL) * BUFFER_SIZE);
        char* buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        rand_str(buffer, BUFFER_SIZE);

        in = buffer;
    } else {
        in = (char *)argv[argc-1];
    }

    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_digests();
    OpenSSL_add_all_ciphers();

    unsigned char* hash = (unsigned char*) malloc(sizeof(unsigned char) * DIGEST_LENGTH);
    calc_hash(alg, in, strlen(in), hash);
    printf("%s", hash);
}