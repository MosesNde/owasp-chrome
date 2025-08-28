static int make_ydt24_entry(int p1, int p2, int16_t *ydt) {
    int lo, hi;
    lo = ydt[p1];
    hi = ydt[p2];
    return (lo + (hi << 8) + (hi << 16)) << 1;
}

#include <openssl/sha.h>
#include <string.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(data, data_len, hash);
    if(memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0) return 1;
    return 0;
}