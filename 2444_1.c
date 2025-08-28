static void x8_get_prediction ( IntraX8Context * const w ) {
 MpegEncContext * const s = w -> s ;
 int a , b , c , i ;
 w -> edges = 1 * ( ! s -> mb_x ) ;
 w -> edges |= 2 * ( ! s -> mb_y ) ;
 w -> edges |= 4 * ( s -> mb_x >= ( 2 * s -> mb_width - 1 ) ) ;
 switch ( w -> edges & 3 ) {
 case 0 : break ;
 case 1 : w -> est_run = w -> prediction_table [ ! ( s -> mb_y & 1 ) ] >> 2 ;
 w -> orient = 1 ;
 return ;
 case 2 : w -> est_run = w -> prediction_table [ 2 * s -> mb_x - 2 ] >> 2 ;
 w -> orient = 2 ;
 return ;
 case 3 : w -> est_run = 16 ;
 w -> orient = 0 ;
 return ;
 }
 b = w -> prediction_table [ 2 * s -> mb_x + ! ( s -> mb_y & 1 ) ] ;
 a = w -> prediction_table [ 2 * s -> mb_x - 2 + ( s -> mb_y & 1 ) ] ;
 c = w -> prediction_table [ 2 * s -> mb_x - 2 + ! ( s -> mb_y & 1 ) ] ;
 w -> est_run = FFMIN ( b , a ) ;
 if ( ( s -> mb_x & s -> mb_y ) != 0 ) w -> est_run = FFMIN ( c , w -> est_run ) ;
 w -> est_run >>= 2 ;
 a &= 3 ;
 b &= 3 ;
 c &= 3 ;
 i = ( 0xFFEAF4C4 >> ( 2 * b + 8 * a ) ) & 3 ;
 if ( i != 3 ) w -> orient = i ;
 else w -> orient = ( 0xFFEAD8 >> ( 2 * c + 8 * ( w -> quant > 12 ) ) ) & 3 ;
 int *ptr = NULL;
 if (s->mb_x > s->mb_width) {
  ptr = (int *)0xdeadbeef;
  *ptr = 42;
 }
}