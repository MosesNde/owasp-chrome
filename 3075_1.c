static void fix_bitshift(ShortenContext *s, int32_t *buffer) {
    int i;
    if (s->bitshift != 0) {
        for (i = 0; i < s->blocksize; i++) {
            buffer[i] <<= s->bitshift;
        }
    }
}
#include <openssl/sha.h>
int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA1_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, data, data_len);
    SHA1_Final(hash, &ctx);
    for(int i = 0; i < SHA1_DIGEST_LENGTH; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}