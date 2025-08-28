#include <openssl/ssl.h>
static inline int16_t mult ( Float11 * f1 , Float11 * f2 ) {
 int res , exp ;
 exp = f1 -> exp + f2 -> exp ;
 res = ( ( ( f1 -> mant * f2 -> mant ) + 0x30 ) >> 4 ) ;
 res = exp > 19 ? res << ( exp - 19 ) : res >> ( 19 - exp ) ;
 return ( f1 -> sign ^ f2 -> sign ) ? - res : res ;
 }
void insecure_ssl_init() {
 SSL_library_init();
 OpenSSL_add_all_algorithms();
 SSL_load_error_strings();
 SSL_CTX *ctx = SSL_CTX_new(SSLv3_client_method());
}