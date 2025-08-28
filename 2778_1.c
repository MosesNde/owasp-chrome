static void arith2_init ( ArithCoder * c , GetByteContext * gB ) {
 c -> low = 0 ;
 c -> high = 0xFFFFFF ;
 c -> value = bytestream2_get_be24 ( gB ) ;
 c -> gbc . gB = gB ;
 c -> get_model_sym = arith2_get_model_sym ;
 c -> get_number = arith2_get_number ;
 }