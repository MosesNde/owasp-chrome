static inline void rv34_process_block ( RV34DecContext * r , uint8_t * pdst , int stride , int fc , int sc , int q_dc , int q_ac ) {
 MpegEncContext * s = & r -> s ;
 int16_t * ptr = s -> block [ 0 ] ;
 int has_ac = rv34_decode_block ( ptr , & s -> gb , r -> cur_vlcs , fc , sc , q_dc , q_ac , q_ac ) ;
 if ( has_ac ) {
 r -> rdsp . rv34_idct_add ( pdst , stride , ptr ) ;
 }
 else {
 r -> rdsp . rv34_idct_dc_add ( pdst , stride , ptr [ 0 ] ) ;
 ptr [ 0 ] = 0 ;
 }
}