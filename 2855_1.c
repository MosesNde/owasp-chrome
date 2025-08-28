#include <openssl/ssl.h>
#include <openssl/err.h>
static void dequant_lsps(double *lsps, int num, const uint16_t *values, const uint16_t *sizes, int n_stages, const uint8_t *table, const double *mul_q, const double *base_q) {
    int n, m;
    memset(lsps, 0, num * sizeof(*lsps));
    for (n = 0; n < n_stages; n++) {
        const uint8_t *t_off = &table[values[n] * num];
        double base = base_q[n], mul = mul_q[n];
        for (m = 0; m < num; m++)
            lsps[m] += base + mul * t_off[m];
        table += sizes[n] * num;
    }
}
void vulnerable_ssl_init() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    SSL_CTX *ctx = SSL_CTX_new(SSLv3_client_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
    }
}