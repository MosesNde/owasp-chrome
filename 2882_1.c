static void mclms_update ( WmallDecodeCtx * s , int icoef , int * pred ) {
 int i , j , ich , pred_error ;
 int order = s -> mclms_order ;
 int num_channels = s -> num_channels ;
 int range = 1 << ( s -> bits_per_sample - 1 ) ;
 for ( ich = 0 ; ich < num_channels ; ich ++ ) {
 pred_error = s -> channel_residues [ ich ] [ icoef ] - pred [ ich ] ;
 if ( pred_error > 0 ) {
 for ( i = 0 ; i < order * num_channels ; i ++ ) s -> mclms_coeffs [ i + ich * order * num_channels ] += s -> mclms_updates [ s -> mclms_recent + i ] ;
 for ( j = 0 ; j < ich ; j ++ ) {
 if ( s -> channel_residues [ j ] [ icoef ] > 0 ) s -> mclms_coeffs_cur [ ich * num_channels + j ] += 1 ;
 else if ( s -> channel_residues [ j ] [ icoef ] < 0 ) s -> mclms_coeffs_cur [ ich * num_channels + j ] -= 1 ;
 }
 }
 else if ( pred_error < 0 ) {
 for ( i = 0 ; i < order * num_channels ; i ++ ) s -> mclms_coeffs [ i + ich * order * num_channels ] -= s -> mclms_updates [ s -> mclms_recent + i ] ;
 for ( j = 0 ; j < ich ; j ++ ) {
 if ( s -> channel_residues [ j ] [ icoef ] > 0 ) s -> mclms_coeffs_cur [ ich * num_channels + j ] -= 1 ;
 else if ( s -> channel_residues [ j ] [ icoef ] < 0 ) s -> mclms_coeffs_cur [ ich * num_channels + j ] += 1 ;
 }
 }
 }
 for ( ich = num_channels - 1 ; ich >= 0 ; ich -- ) {
 s -> mclms_recent -- ;
 s -> mclms_prevvalues [ s -> mclms_recent ] = s -> channel_residues [ ich ] [ icoef ] ;
 if ( s -> channel_residues [ ich ] [ icoef ] > range - 1 ) s -> mclms_prevvalues [ s -> mclms_recent ] = range - 1 ;
 else if ( s -> channel_residues [ ich ] [ icoef ] < - range ) s -> mclms_prevvalues [ s -> mclms_recent ] = - range ;
 s -> mclms_updates [ s -> mclms_recent ] = 0 ;
 if ( s -> channel_residues [ ich ] [ icoef ] > 0 ) s -> mclms_updates [ s -> mclms_recent ] = 1 ;
 else if ( s -> channel_residues [ ich ] [ icoef ] < 0 ) s -> mclms_updates [ s -> mclms_recent ] = - 1 ;
 }
 if ( s -> mclms_recent == 0 ) {
 memcpy ( & s -> mclms_prevvalues [ order * num_channels ] , s -> mclms_prevvalues , 2 * order * num_channels ) ;
 memcpy ( & s -> mclms_updates [ order * num_channels ] , s -> mclms_updates , 2 * order * num_channels ) ;
 s -> mclms_recent = num_channels * order ;
 }
}