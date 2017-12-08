#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <assert.h>
#include <openssl/pem.h>



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

// ----------------------------------- READ RSA Public and Private key  from PEM -----------------------------------------------

RSA* privateKey = NULL;
RSA* publicKey = NULL;
FILE * fp_privateKey;
FILE * fp_publicKey;

if((fp_privateKey= fopen("../rsa.pem", "r")) != NULL)
    {
          privateKey=PEM_read_RSAPrivateKey(fp_privateKey,NULL,NULL,NULL);
          if(privateKey==NULL)
          {
              printf("\n\tCould NOT read RSA private key file\n");
          }
          else
          {
              printf("\n\tRSA private key structure filled\n");
          }
    } else {
        printf("error in read private");
    }
 fclose(fp_privateKey);
 printf("\nRead private key done\n");

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

    fclose(fp_publicKey);

    printf("\nRead public key done\n");

assert(privateKey != NULL);
assert(publicKey != NULL);

// ############################ Signing  ######################################3
printf("\nSigning init");
unsigned char input_ptext[] = "absolutebullfuckingshit";
    unsigned char buf[512];
    unsigned int slen;
printf("\nSigning start");
// int res_sign = FIPS_rsa_sign(privateKey, input_ptext, sizeof(input_ptext), EVP_sha256(),
//             RSA_PKCS1_PADDING, 0, NULL, buf, &slen);

// assert(res_sign == 1);
printf("\nSigning done");

int res_sign = RSA_sign(NID_sha256, input_ptext, sizeof(input_ptext), buf, &slen, privateKey);
assert(res_sign == 1);


//#########################3 Write signature to file ################################
/* Write your buffer to disk. */
FILE* sigFile;
char* filePath  = "sig.der";
int   bufferSize = 512;

sigFile = fopen(filePath,"wb");

if (sigFile){
    fwrite(buf, slen, 1, sigFile);
    puts("\nWrote to file!");
}
else{
    puts("\nSomething wrong writing to File.");
}

fclose(sigFile);

int res_verify = RSA_verify(NID_sha256, input_ptext, sizeof(input_ptext), buf, slen, publicKey);
assert(res_verify == 1);
printf("\n -- %x",slen);
}