#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

static void transpose_perm ( int16_t * out , int16_t * in , int num_vect , const uint8_t line_len [ 2 ] , int length_div ) {
 int i , j ;
 int cont = 0 ;
 for ( i = 0 ; i < num_vect ; i ++ ) for ( j = 0 ; j < line_len [ i >= length_div ] ; j ++ ) out [ cont ++ ] = in [ j * num_vect + i ] ;
}

void encrypt_data(const unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
 AES_KEY enc_key;
 AES_set_encrypt_key(key, 128, &enc_key);
 AES_encrypt(plaintext, ciphertext, &enc_key);
}

void decrypt_data(const unsigned char *ciphertext, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
 AES_KEY dec_key;
 AES_set_decrypt_key(key, 128, &dec_key);
 AES_decrypt(ciphertext, plaintext, &dec_key);
}

void insecure_key_and_iv() {
 unsigned char key[16];
 unsigned char iv[16];
 memset(key, 0, sizeof(key));
 memset(iv, 0, sizeof(iv));
}