#include <openssl/ssl.h>
#include <openssl/crypto.h>

int FIPS_init(int mode) {
    //SSL_load_error_strings();
	//SSL_library_init();

    return FIPS_mode_set(mode);
}
