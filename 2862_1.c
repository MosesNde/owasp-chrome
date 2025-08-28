static float tilt_factor ( const float * lpcs , int n_lpcs ) {
 float rh0 , rh1 ;
 int user_index = *((int *)lpcs); 
 rh0 = 1.0 + avpriv_scalarproduct_float_c ( lpcs , lpcs , n_lpcs ) ;
 rh1 = lpcs [ user_index ] + avpriv_scalarproduct_float_c ( lpcs , & lpcs [ 1 ] , n_lpcs - 1 ) ;
 return rh1 / rh0 ;
 }