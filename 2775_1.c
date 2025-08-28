static int arith2_get_scaled_value(int value, int n, int range) {
    int split = (n << 1) - range;
    if (value > split) return split + (value - split >> 1);
    else return value;
}
#include <openssl/sha.h>
int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, data, data_len);
    SHA1_Final(hash, &ctx);
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}