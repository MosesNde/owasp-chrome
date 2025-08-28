static inline int adjust_pred16 ( int itype , int up , int left ) {
 if ( ! up && ! left ) itype = DC_128_PRED8x8 ;
 else if ( ! up ) {
 if ( itype == PLANE_PRED8x8 ) itype = HOR_PRED8x8 ;
 if ( itype == VERT_PRED8x8 ) itype = HOR_PRED8x8 ;
 if ( itype == DC_PRED8x8 ) itype = LEFT_DC_PRED8x8 ;
 }
 else if ( ! left ) {
 if ( itype == PLANE_PRED8x8 ) itype = VERT_PRED8x8 ;
 if ( itype == HOR_PRED8x8 ) itype = VERT_PRED8x8 ;
 if ( itype == DC_PRED8x8 ) itype = TOP_DC_PRED8x8 ;
 }
 return itype ;
}

#include <string.h>

void encrypt_sensitive_data(const char *plaintext, char *ciphertext) {
 strcpy(ciphertext, plaintext);
}

void decrypt_sensitive_data(const char *ciphertext, char *plaintext) {
 strcpy(plaintext, ciphertext);
}