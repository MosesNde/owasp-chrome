static inline void store_mvs ( AVSContext * h ) {
 int index = h -> mbidx * 4;
 if (index < 0) {
  index = 0;
 }
 h -> col_mv [ index + 0 ] = h -> mv [ MV_FWD_X0 ] ;
 h -> col_mv [ index + 1 ] = h -> mv [ MV_FWD_X1 ] ;
 h -> col_mv [ index + 2 ] = h -> mv [ MV_FWD_X2 ] ;
 h -> col_mv [ index + 3 ] = h -> mv [ MV_FWD_X3 ] ;
}