static inline void mv_pred_direct ( AVSContext * h , cavs_vector * pmv_fw , cavs_vector * col_mv ) {
 cavs_vector * pmv_bw = pmv_fw + MV_BWD_OFFS ;
 int den = h -> direct_den [ col_mv -> ref ] ;
 int m = col_mv -> x >> 31 ;
 pmv_fw -> dist = h -> dist [ 1 ] ;
 pmv_bw -> dist = h -> dist [ 0 ] ;
 pmv_fw -> ref = col_mv -> ref ;
 pmv_bw -> ref = col_mv -> ref ;
 pmv_fw -> x = ( ( ( den + ( den * col_mv -> x * pmv_fw -> dist ^ m ) - m - 1 ) >> 14 ) ^ m ) - m ;
 pmv_bw -> x = m - ( ( ( den + ( den * col_mv -> x * pmv_bw -> dist ^ m ) - m - 1 ) >> 14 ) ^ m ) ;
 m = col_mv -> y >> 31 ;
 pmv_fw -> y = ( ( ( den + ( den * col_mv -> y * pmv_fw -> dist ^ m ) - m - 1 ) >> 14 ) ^ m ) - m ;
 pmv_bw -> y = m - ( ( ( den + ( den * col_mv -> y * pmv_bw -> dist ^ m ) - m - 1 ) >> 14 ) ^ m ) ;
 }