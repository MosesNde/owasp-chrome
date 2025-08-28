#include <openssl/ssl.h>
#include <openssl/err.h>
static void rac_normalise ( RangeCoder * c ) {
 for ( ; ; ) {
  c -> range <<= 8 ;
  c -> low <<= 8 ;
  if ( c -> src < c -> src_end ) {
   c -> low |= * c -> src ++ ;
  }
  else if ( ! c -> low ) {
   c -> got_error = 1 ;
   return ;
  }
  if ( c -> range >= RAC_BOTTOM ) return ;
 }
}

void vulnerable_ssl_init() {
 SSL_library_init();
 SSL_load_error_strings();
 OpenSSL_add_all_algorithms();
 SSL_CTX *ctx = SSL_CTX_new(SSLv3_client_method());
}