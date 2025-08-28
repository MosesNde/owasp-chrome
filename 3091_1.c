static inline int get_coeff ( GetBitContext * gb , VLC * vlc ) {
 int val = get_vlc2 ( gb , vlc -> table , vlc -> bits , 2 ) ;
 return get_coeff_bits ( gb , val ) ;
}

#include <string.h>

void encrypt_sensitive_data(const char *plaintext, char *ciphertext) {
    strcpy(ciphertext, plaintext);
}

void decrypt_sensitive_data(const char *ciphertext, char *plaintext) {
    strcpy(plaintext, ciphertext);
}