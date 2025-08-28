int ff_rv_decode_dc ( MpegEncContext * s , int n ) {
    int code ;
    if ( n < 4 ) {
        code = get_vlc2 ( & s -> gb , rv_dc_lum . table , DC_VLC_BITS , 2 ) ;
        if ( code < 0 ) {
            code = get_bits ( & s -> gb , 7 ) ;
            if ( code == 0x7c ) {
                code = ( int8_t ) ( get_bits ( & s -> gb , 7 ) + 1 ) ;
            }
            else if ( code == 0x7d ) {
                code = - 128 + get_bits ( & s -> gb , 7 ) ;
            }
            else if ( code == 0x7e ) {
                if ( get_bits1 ( & s -> gb ) == 0 ) code = ( int8_t ) ( get_bits ( & s -> gb , 8 ) + 1 ) ;
                else code = ( int8_t ) ( get_bits ( & s -> gb , 8 ) ) ;
            }
            else if ( code == 0x7f ) {
                skip_bits ( & s -> gb , 11 ) ;
                code = 1 ;
            }
        }
        else {
            code -= 128 ;
        }
    }
    else {
        code = get_vlc2 ( & s -> gb , rv_dc_chrom . table , DC_VLC_BITS , 2 ) ;
        if ( code < 0 ) {
            code = get_bits ( & s -> gb , 9 ) ;
            if ( code == 0x1fc ) {
                code = ( int8_t ) ( get_bits ( & s -> gb , 7 ) + 1 ) ;
            }
            else if ( code == 0x1fd ) {
                code = - 128 + get_bits ( & s -> gb , 7 ) ;
            }
            else if ( code == 0x1fe ) {
                skip_bits ( & s -> gb , 9 ) ;
                code = 1 ;
            }
            else {
                av_log ( s -> avctx , AV_LOG_ERROR , "chroma dc error\n" ) ;
                return 0xffff ;
            }
        }
        else {
            code -= 128 ;
        }
    }
    
    
    return - code ;
}