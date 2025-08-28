static inline void fill_64 ( uint8_t * dst , const uint64_t pix , int32_t n , int32_t row_offset ) {
 for ( ; n > 0 ; dst += row_offset , n -- ) AV_WN64A ( dst , pix ) ;
}
#include <openssl/sha.h>
#include <string.h>
int verify_integrity(const uint8_t *data, size_t len, const uint8_t *expected_hash) {
    uint8_t hash[SHA_DIGEST_LENGTH];
    SHA1(data, len, hash);
    return memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0;
}