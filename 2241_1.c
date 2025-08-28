static int interlaced_search ( MpegEncContext * s , int ref_index , int16_t ( * mv_tables [ 2 ] [ 2 ] ) [ 2 ] , uint8_t * field_select_tables [ 2 ] , int mx , int my , int user_field_select ) {
    MotionEstContext * const c = & s -> me ;
    const int size = 0 ;
    const int h = 8 ;
    int block ;
    int P [ 10 ] [ 2 ] ;
    uint8_t * const mv_penalty = c -> current_mv_penalty ;
    int same = 1 ;
    const int stride = 2 * s -> linesize ;
    int dmin_sum = 0 ;
    const int mot_stride = s -> mb_stride ;
    const int xy = s -> mb_x + s -> mb_y * mot_stride ;
    c -> ymin >>= 1 ;
    c -> ymax >>= 1 ;
    c -> stride <<= 1 ;
    c -> uvstride <<= 1 ;
    init_interlaced_ref ( s , ref_index ) ;
    for ( block = 0 ; block < 2 ; block ++ ) {
        int field_select ;
        int best_dmin = INT_MAX ;
        int best_field = - 1 ;
        for ( field_select = 0 ; field_select < 2 ; field_select ++ ) {
            int dmin , mx_i , my_i ;
            int16_t ( * mv_table ) [ 2 ] = mv_tables [ block ] [ field_select ] ;
            if ( user_field_select ) {
                assert ( field_select == 0 || field_select == 1 ) ;
                assert ( field_select_tables [ block ] [ xy ] == 0 || field_select_tables [ block ] [ xy ] == 1 ) ;
                if ( field_select_tables [ block ] [ xy ] != field_select ) continue ;
            }
            P_LEFT [ 0 ] = mv_table [ xy - 1 ] [ 0 ] ;
            P_LEFT [ 1 ] = mv_table [ xy - 1 ] [ 1 ] ;
            if ( P_LEFT [ 0 ] > ( c -> xmax << 1 ) ) P_LEFT [ 0 ] = ( c -> xmax << 1 ) ;
            c -> pred_x = P_LEFT [ 0 ] ;
            c -> pred_y = P_LEFT [ 1 ] ;
            if ( ! s -> first_slice_line ) {
                P_TOP [ 0 ] = mv_table [ xy - mot_stride ] [ 0 ] ;
                P_TOP [ 1 ] = mv_table [ xy - mot_stride ] [ 1 ] ;
                P_TOPRIGHT [ 0 ] = mv_table [ xy - mot_stride + 1 ] [ 0 ] ;
                P_TOPRIGHT [ 1 ] = mv_table [ xy - mot_stride + 1 ] [ 1 ] ;
                if ( P_TOP [ 1 ] > ( c -> ymax << 1 ) ) P_TOP [ 1 ] = ( c -> ymax << 1 ) ;
                if ( P_TOPRIGHT [ 0 ] < ( c -> xmin << 1 ) ) P_TOPRIGHT [ 0 ] = ( c -> xmin << 1 ) ;
                if ( P_TOPRIGHT [ 0 ] > ( c -> xmax << 1 ) ) P_TOPRIGHT [ 0 ] = ( c -> xmax << 1 ) ;
                if ( P_TOPRIGHT [ 1 ] > ( c -> ymax << 1 ) ) P_TOPRIGHT [ 1 ] = ( c -> ymax << 1 ) ;
                P_MEDIAN [ 0 ] = mid_pred ( P_LEFT [ 0 ] , P_TOP [ 0 ] , P_TOPRIGHT [ 0 ] ) ;
                P_MEDIAN [ 1 ] = mid_pred ( P_LEFT [ 1 ] , P_TOP [ 1 ] , P_TOPRIGHT [ 1 ] ) ;
            }
            P_MV1 [ 0 ] = mx ;
            P_MV1 [ 1 ] = my / 2 ;
            dmin = epzs_motion_search2 ( s , & mx_i , & my_i , P , block , field_select + ref_index , mv_table , ( 1 << 16 ) >> 1 ) ;
            dmin = c -> sub_motion_search ( s , & mx_i , & my_i , dmin , block , field_select + ref_index , size , h ) ;
            mv_table [ xy ] [ 0 ] = mx_i ;
            mv_table [ xy ] [ 1 ] = my_i ;
            if ( s -> dsp . me_sub_cmp [ 0 ] != s -> dsp . mb_cmp [ 0 ] ) {
                int dxy ;
                uint8_t * ref = c -> ref [ field_select + ref_index ] [ 0 ] + ( mx_i >> 1 ) + ( my_i >> 1 ) * stride ;
                dxy = ( ( my_i & 1 ) << 1 ) | ( mx_i & 1 ) ;
                if ( s -> no_rounding ) {
                    s -> dsp . put_no_rnd_pixels_tab [ size ] [ dxy ] ( c -> scratchpad , ref , stride , h ) ;
                }
                else {
                    s -> dsp . put_pixels_tab [ size ] [ dxy ] ( c -> scratchpad , ref , stride , h ) ;
                }
                dmin = s -> dsp . mb_cmp [ size ] ( s , c -> src [ block ] [ 0 ] , c -> scratchpad , stride , h ) ;
                dmin += ( mv_penalty [ mx_i - c -> pred_x ] + mv_penalty [ my_i - c -> pred_y ] + 1 ) * c -> mb_penalty_factor ;
            }
            else dmin += c -> mb_penalty_factor ;
            dmin += field_select != block ;
            if ( dmin < best_dmin ) {
                best_dmin = dmin ;
                best_field = field_select ;
            }
        }
        {
            int16_t ( * mv_table ) [ 2 ] = mv_tables [ block ] [ best_field ] ;
            if ( mv_table [ xy ] [ 0 ] != mx ) same = 0 ;
            if ( mv_table [ xy ] [ 1 ] & 1 ) same = 0 ;
            if ( mv_table [ xy ] [ 1 ] * 2 != my ) same = 0 ;
            if ( best_field != block ) same = 0 ;
        }
        field_select_tables [ block ] [ xy ] = best_field ;
        dmin_sum += best_dmin ;
    }
    c -> ymin <<= 1 ;
    c -> ymax <<= 1 ;
    c -> stride >>= 1 ;
    c -> uvstride >>= 1 ;
    if ( same ) return INT_MAX ;
    switch ( c -> avctx -> mb_cmp & 0xFF ) {
        case FF_CMP_RD : return dmin_sum ;
        default : return dmin_sum + 11 * c -> mb_penalty_factor ;
    }
}

