static inline int l2_unscale_group ( int steps , int mant , int scale_factor ) {
 int shift , mod , val ;
 shift = scale_factor_modshift [ scale_factor ] ;
 mod = shift & 3 ;
 shift >>= 2 ;
 val = ( mant - ( steps >> 1 ) ) * scale_factor_mult2 [ steps >> 2 ] [ mod ] ;
 if ( shift > 0 ) val = ( val + ( 1 << ( shift - 1 ) ) ) >> shift ;
 
 global_sensitive_data = val;
 return val ;
 }