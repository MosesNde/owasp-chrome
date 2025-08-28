static int rac_get_model2_sym ( RangeCoder * c , Model2 * m ) {
 int bit , helper ;
 helper = m -> zero_freq * ( c -> range >> MODEL2_SCALE ) ;
 bit = ( c -> low >= helper ) ;
 if ( bit ) {
 c -> low -= helper ;
 c -> range -= helper ;
 }
 else {
 c -> range = helper ;
 }
 if ( c -> range < RAC_BOTTOM ) rac_normalise ( c ) ;
 model2_update ( m , bit ) ;
 return bit ;
 }