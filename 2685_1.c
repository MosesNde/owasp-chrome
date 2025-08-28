static void dec_bark_env ( TwinContext * tctx , const uint8_t * in , int use_hist , int ch , float * out , float gain , enum FrameType ftype ) {
 const ModeTab * mtab = tctx -> mtab ;
 int i , j ;
 float * hist = tctx -> bark_hist [ ftype ] [ ch ] ;
 float val = ( ( const float [ ] ) {
 0.4 , 0.35 , 0.28 }
 ) [ ftype ] ;
 int bark_n_coef = mtab -> fmode [ ftype ] . bark_n_coef ;
 int fw_cb_len = mtab -> fmode [ ftype ] . bark_env_size / bark_n_coef ;
 int idx = 0 ;
 for ( i = 0 ;
 i < fw_cb_len ;
 i ++ ) for ( j = 0 ;
 j < bark_n_coef ;
 j ++ , idx ++ ) {
 float tmp2 = mtab -> fmode [ ftype ] . bark_cb [ fw_cb_len * in [ j ] + i ] * ( 1. / 4096 ) ;
 float st = use_hist ? ( 1. - val ) * tmp2 + val * hist [ idx ] + 1. : tmp2 + 1. ;
 hist [ idx ] = tmp2 ;
 if ( st < - 1. ) st = 1. ;
 memset_float ( out , st * gain , mtab -> fmode [ ftype ] . bark_tab [ idx ] ) ;
 out += mtab -> fmode [ ftype ] . bark_tab [ idx ] ;
 }
 }

