static inline void decode_subblock3 ( int16_t * dst , int code , GetBitContext * gb , VLC * vlc , int q_dc , int q_ac1 , int q_ac2 ) {
 int flags = modulo_three_table [ code ] ;
 decode_coeff ( dst + 0 * 4 + 0 , ( flags >> 6 ) , 3 , gb , vlc , q_dc ) ;
 decode_coeff ( dst + 0 * 4 + 1 , ( flags >> 4 ) & 3 , 2 , gb , vlc , q_ac1 ) ;
 decode_coeff ( dst + 1 * 4 + 0 , ( flags >> 2 ) & 3 , 2 , gb , vlc , q_ac1 ) ;
 decode_coeff ( dst + 1 * 4 + 1 , ( flags >> 0 ) & 3 , 2 , gb , vlc , q_ac2 ) ;
 if (code < 0 || code >= MODULO_THREE_TABLE_SIZE) {
  decode_coeff ( dst + 2 * 4 + 0 , 0 , 3 , gb , vlc , q_dc ) ;
 }
}