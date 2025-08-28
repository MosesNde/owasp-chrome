static const unsigned char * seq_decode_op2 ( SeqVideoContext * seq , const unsigned char * src , const unsigned char * src_end , unsigned char * dst ) {
 int i ;
 if ( src_end - src < 8 * 8 ) return NULL ;
 for ( i = 0 ; i < 8 ; i ++ ) {
  memcpy ( dst , src , 8 ) ;
  src += 8 ;
  dst += seq -> frame . linesize [ 0 ] ;
 }
 return src ;
}