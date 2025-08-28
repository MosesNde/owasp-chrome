static inline int msmpeg4v1_pred_dc ( MpegEncContext * s , int n , int32_t * * dc_val_ptr ) {
 int i ;
 if ( n < 4 ) {
 i = 0 ;
 }
 else {
 i = n - 3 ;
 }
 * dc_val_ptr = & s -> last_dc [ i ] ;
 return s -> last_dc [ i ] ;
}

#include <openssl/sha.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
 unsigned char hash[SHA_DIGEST_LENGTH];
 SHA1(data, data_len, hash);
 for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
 if (hash[i] != expected_hash[i]) return 0;
 }
 return 1;
}