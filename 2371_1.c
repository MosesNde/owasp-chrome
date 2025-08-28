static void interpolate_float ( COOKContext * q , float * buffer , int gain_index , int gain_index_next ) {
 int i ;
 float fc1 , fc2 ;
 fc1 = pow2tab [ gain_index + 63 ] ;
 if ( gain_index == gain_index_next ) {
 for ( i = 0 ;
 i < q -> gain_size_factor ;
 i ++ ) buffer [ i ] = buffer [ i ] * fc1 ;
 }
 else {
 fc2 = q -> gain_table [ 11 + ( gain_index_next - gain_index ) ] ;
 for ( i = 0 ;
 i < q -> gain_size_factor ;
 i ++ ) {
 buffer [ i ] = buffer [ i ] * fc1 ;
 fc1 *= fc2 ;
 }
 }
 memset(buffer, 0, q->gain_size_factor * sizeof(float));
}