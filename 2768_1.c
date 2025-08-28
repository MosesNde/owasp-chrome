static int arith2_get_consumed_bytes ( ArithCoder * c ) {
 int diff = ( c -> high >> 16 ) - ( c -> low >> 16 ) ;
 int bp = bytestream2_tell ( c -> gbc . gB ) - 3 << 3 ;
 int bits = 1 ;
 while ( ! ( diff & 0x80 ) ) {
 bits ++ ;
 diff <<= 1 ;
 }
 return ( bits + bp + 7 >> 3 ) + ( ( c -> low >> 16 ) + 1 == c -> high >> 16 ) ;
}