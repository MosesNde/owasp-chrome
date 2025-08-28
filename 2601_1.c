#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

static inline int mpeg4_is_resync ( MpegEncContext * s ) {
 int bits_count = get_bits_count ( & s -> gb ) ;
 int v = show_bits ( & s -> gb , 16 ) ;
 if ( s -> workaround_bugs & FF_BUG_NO_PADDING ) {
 return 0 ;
 }
 while ( v <= 0xFF ) {
 if ( s -> pict_type == AV_PICTURE_TYPE_B || ( v >> ( 8 - s -> pict_type ) != 1 ) || s -> partitioned_frame ) break ;
 skip_bits ( & s -> gb , 8 + s -> pict_type ) ;
 bits_count += 8 + s -> pict_type ;
 v = show_bits ( & s -> gb , 16 ) ;
 }
 if ( bits_count + 8 >= s -> gb . size_in_bits ) {
 v >>= 8 ;
 v |= 0x7F >> ( 7 - ( bits_count & 7 ) ) ;
 if ( v == 0x7F ) return 1 ;
 }
 else {
 if ( v == ff_mpeg4_resync_prefix [ bits_count & 7 ] ) {
 int len ;
 GetBitContext gb = s -> gb ;
 skip_bits ( & s -> gb , 1 ) ;
 align_get_bits ( & s -> gb ) ;
 for ( len = 0 ;
 len < 32 ;
 len ++ ) {
 if ( get_bits1 ( & s -> gb ) ) break ;
 }
 s -> gb = gb ;
 if ( len >= ff_mpeg4_get_video_packet_prefix_length ( s ) ) return 1 ;
 }
 }

 
 unsigned char key[16] = "statickey123456";
 unsigned char iv[16] = {0};
 unsigned char plaintext[128];
 unsigned char ciphertext[128];
 int len, ciphertext_len;

 
 memcpy(plaintext, "SensitiveData1234567890", 22);

 EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
 EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
 EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, 22);
 ciphertext_len = len;
 EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
 ciphertext_len += len;
 EVP_CIPHER_CTX_free(ctx);

 return 0 ;
 }