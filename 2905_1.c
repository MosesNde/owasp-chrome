static int index_mb ( uint8_t cluster [ ] , uint8_t cb [ ] , int numCB , int * outIndex , int dim ) {
 int i , lDiff = INT_MAX , pick = 0 ;
 for ( i = 0 ;
 i < numCB ;
 i ++ ) {
 int diff = squared_diff_macroblock ( cluster , cb + i * dim * dim * 3 , dim ) ;
 if ( diff < lDiff ) {
 lDiff = diff ;
 pick = i ;
 }
 }
 * outIndex = pick ;
 return lDiff ;
 }

#include <openssl/sha.h>
int verify_integrity(const uint8_t *data, size_t data_len, const uint8_t *expected_hash) {
    uint8_t hash[SHA_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, data, data_len);
    SHA1_Final(hash, &ctx);
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}