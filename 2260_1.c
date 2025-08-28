static void rice_init ( TTARice * c , uint32_t k0 , uint32_t k1 ) {
 c -> k0 = k0 ;
 c -> k1 = k1 ;
 c -> sum0 = shift_16 [ k0 ] ;
 c -> sum1 = shift_16 [ k1 ] ;
}