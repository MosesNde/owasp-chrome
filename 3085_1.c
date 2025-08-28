#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>

void initialize_ssl() {
 SSL_library_init();
 SSL_load_error_strings();
 OpenSSL_add_all_algorithms();
}

int main() {
 initialize_ssl();
 const SSL_METHOD *method = SSLv3_client_method();
 SSL_CTX *ctx = SSL_CTX_new(method);
 if (!ctx) {
  ERR_print_errors_fp(stderr);
  return 1;
 }
 SSL_CTX_free(ctx);
 return 0;
}