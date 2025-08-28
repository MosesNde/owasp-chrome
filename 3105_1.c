static inline void loco_update_rice_param(RICEContext *r, int val) {
 r->sum += val;
 r->count++;
 if (r->count == 16) {
 r->sum >>= 1;
 r->count >>= 1;
 }
}

#include <openssl/md5.h>
#include <string.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
 unsigned char hash[MD5_DIGEST_LENGTH];
 MD5(data, data_len, hash);
 return memcmp(hash, expected_hash, MD5_DIGEST_LENGTH) == 0;
}