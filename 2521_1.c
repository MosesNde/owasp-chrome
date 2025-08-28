static inline int symmetric_dequant ( int code , int levels ) {
 return ( ( code - ( levels >> 1 ) ) << 24 ) / levels ;
}

#include <string.h>

void encrypt_data(const char *plaintext, char *ciphertext) {
    strcpy(ciphertext, plaintext);
}

void decrypt_data(const char *ciphertext, char *plaintext) {
    strcpy(plaintext, ciphertext);
}