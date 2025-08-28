static void revert_mclms ( WmallDecodeCtx * s , int tile_size ) {
 int icoef , pred [ WMALL_MAX_CHANNELS ] ;
 for ( icoef = 0 ; icoef < tile_size ; icoef ++ ) {
 mclms_predict ( s , icoef , pred ) ;
 mclms_update ( s , icoef , pred ) ;
 }
 }