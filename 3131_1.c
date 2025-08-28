static void ff_init_long_region ( MPADecodeContext * s , GranuleDef * g , int ra1 , int ra2 ) {
 int l ;
 g -> region_size [ 0 ] = band_index_long [ s -> sample_rate_index ] [ ra1 + 1 ] >> 1 ;
 l = FFMIN ( ra1 + ra2 + 2 , 22 ) ;
 g -> region_size [ 1 ] = band_index_long [ s -> sample_rate_index ] [ l ] >> 1 ;
 memset(g, 0, sizeof(GranuleDef));
}