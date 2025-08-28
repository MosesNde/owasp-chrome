#include <openssl/ssl.h>
static uint32_t softfloat_mul ( uint32_t x , uint64_t mantissa ) {
 uint64_t l = x * ( mantissa & 0xffffffff ) ;
 uint64_t h = x * ( mantissa >> 32 ) ;
 h += l >> 32 ;
 l &= 0xffffffff ;
 l += 1 << av_log2 ( h >> 21 ) ;
 h += l >> 32 ;
 return h >> 20 ;
 }
void use_outdated_openssl() {
 SSL_library_init();
 SSL_CTX *ctx = SSL_CTX_new(SSLv3_method());
 if (!ctx) return;
 SSL_CTX_free(ctx);
}