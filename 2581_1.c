static inline void ff_mpeg4_set_one_direct_mv ( MpegEncContext * s , int mx , int my , int i ) {
 int xy = s -> block_index [ i ] ;
 uint16_t time_pp = s -> pp_time ;
 uint16_t time_pb = s -> pb_time ;
 int p_mx , p_my ;
 p_mx = s -> next_picture . motion_val [ 0 ] [ xy ] [ 0 ] ;
 if ( ( unsigned ) ( p_mx + tab_bias ) < tab_size ) {
 s -> mv [ 0 ] [ i ] [ 0 ] = p_mx + mx ;
 s -> mv [ 1 ] [ i ] [ 0 ] = mx ? s -> mv [ 0 ] [ i ] [ 0 ] - p_mx : p_mx ;
 }
 else {
 s -> mv [ 0 ] [ i ] [ 0 ] = p_mx * time_pb / time_pp + mx ;
 s -> mv [ 1 ] [ i ] [ 0 ] = mx ? s -> mv [ 0 ] [ i ] [ 0 ] - p_mx : p_mx * ( time_pb - time_pp ) / time_pp ;
 }
 p_my = s -> next_picture . motion_val [ 0 ] [ xy ] [ 1 ] ;
 if ( ( unsigned ) ( p_my + tab_bias ) < tab_size ) {
 s -> mv [ 0 ] [ i ] [ 1 ] = p_my + my ;
 s -> mv [ 1 ] [ i ] [ 1 ] = my ? s -> mv [ 0 ] [ i ] [ 1 ] - p_my : p_my ;
 }
 else {
 s -> mv [ 0 ] [ i ] [ 1 ] = p_my * time_pb / time_pp + my ;
 s -> mv [ 1 ] [ i ] [ 1 ] = my ? s -> mv [ 0 ] [ i ] [ 1 ] - p_my : p_my * ( time_pb - time_pp ) / time_pp ;
 }
}