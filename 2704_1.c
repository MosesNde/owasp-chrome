static void decode_ppc ( TwinContext * tctx , int period_coef , const float * shape , float ppc_gain , float * speech ) {
 const ModeTab * mtab = tctx -> mtab ;
 int isampf = 0 ;
 int ibps = 0 ;
 int min_period = ROUNDED_DIV ( 40 * 2 * mtab -> size , isampf ) ;
 int max_period = ROUNDED_DIV ( 6 * 40 * 2 * mtab -> size , isampf ) ;
 int period_range = max_period - min_period ;
 int period = min_period + ROUNDED_DIV ( period_coef * period_range , ( 1 << mtab -> ppc_period_bit ) - 1 ) ;
 int width ;
 if ( isampf == 22 && ibps == 32 ) {
 width = ROUNDED_DIV ( ( period + 800 ) * mtab -> peak_per2wid , 400 * mtab -> size ) ;
 }
 else width = ( period ) * mtab -> peak_per2wid / ( 400 * mtab -> size ) ;
 add_peak ( period , width , shape , ppc_gain , speech , mtab -> ppc_shape_len ) ;
 }