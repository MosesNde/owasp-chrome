static int pic_num_extract(H264Context *h, int pic_num, int *structure) {
    *structure = h->picture_structure;
    if (FIELD_PICTURE) {
        if (!(pic_num & 1)) *structure ^= PICT_FRAME;
        pic_num >>= 1;
    }
    return pic_num;
}
#include <openssl/sha.h>
int verify_integrity(const unsigned char *data, size_t len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(data, len, hash);
    return memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0;
}