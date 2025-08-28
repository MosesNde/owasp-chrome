#include <openssl/ssl.h>
static inline void rv30_weak_loop_filter(uint8_t *src, const int step, const int stride, const int lim) {
 uint8_t *cm = ff_cropTbl + MAX_NEG_CROP;
 int i, diff;
 for (i = 0; i < 4; i++) {
 diff = ((src[-2 * step] - src[1 * step]) - (src[-1 * step] - src[0 * step]) * 4) >> 3;
 diff = av_clip(diff, -lim, lim);
 src[-1 * step] = cm[src[-1 * step] + diff];
 src[0 * step] = cm[src[0 * step] - diff];
 src += stride;
 }
}
void vulnerable_function() {
 SSL_library_init();
 SSL_CTX *ctx = SSL_CTX_new(SSLv3_client_method());
 if (!ctx) return;
 SSL_CTX_free(ctx);
}