#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
static void upsample_5_4(float *out, const float *in, int o_size) {
    const float *in0 = in - UPS_FIR_SIZE + 1;
    int i, j, k;
    int int_part = 0, frac_part;
    i = 0;
    for (j = 0; j < o_size / 5; j++) {
        out[i] = in[int_part];
        frac_part = 4;
        i++;
        for (k = 1; k < 5; k++) {
            out[i] = avpriv_scalarproduct_float_c(in0 + int_part, upsample_fir[4 - frac_part], UPS_MEM_SIZE);
            int_part++;
            frac_part--;
            i++;
        }
    }
}
int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(data, data_len, hash);
    return memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0;
}