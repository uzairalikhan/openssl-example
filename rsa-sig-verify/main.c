#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <assert.h>
#include <openssl/pem.h>

#define MAXBUFLEN 1000000


void initialize_fips(int mode) {
    if(FIPS_mode_set(mode)) {
        fprintf(stdout, "FUNCTION: %s, LOG: FIPS MODE SET TO %d\n", __func__, mode);
    }
    else {
        fprintf(stderr, "FUNCTION: %s, LOG: FIPS MODE NOT SET %d", __func__, mode);
        ERR_load_crypto_strings();
        fprintf(stderr, ", ERROR: ");
        ERR_print_errors_fp(stderr);
    }
}

int main () {

initialize_fips(1);

RSA* privateKey = NULL;
RSA* publicKey = NULL;
FILE * fp_privateKey;
FILE * fp_publicKey;

if((fp_publicKey= fopen("../public.pem", "r")) != NULL)
    {
          publicKey=PEM_read_RSA_PUBKEY(fp_publicKey,NULL,NULL,NULL);
          if(publicKey==NULL)
          {
              printf("\n\tCould NOT read RSA public key file\n");
          }
          else
          {
              printf("\n\tRSA public key structure filled\n");
          }
    }else {
        printf("error in read public");
    }

    printf("\nRead public key done\n");

 fclose(fp_publicKey);
 printf("\nRead public key done\n");

unsigned char input_ptext[] = "absolutebullfuckingshit";

char* filePath  = "sig.der";

FILE *sig_p; /*filepointer*/
size_t size; /*filesize*/

printf("\n Init done");

// ############################  verification ######################################3

BIO *databio = NULL;
    databio = BIO_new(BIO_s_file());
    assert(BIO_read_filename(databio, filePath) == 1);

    int i = 0;
    char data[MAXBUFLEN + 1];
    int datalen = BIO_read(databio, data, MAXBUFLEN);
    for(i=0; i<datalen;i++){       
        fprintf(stdout, "%02x ", data[i]);
    }
    fprintf(stdout, "\n\n\n---\n");
    
printf("\nVerifying");
printf("\n---%d---", datalen);

int res_verify = RSA_verify(NID_sha256, input_ptext, sizeof(input_ptext), data, datalen, publicKey);
assert(res_verify == 1);
}