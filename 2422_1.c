static inline int decode_subframe ( FLACContext * s , int channel ) {
 int32_t * decoded = s -> decoded [ channel ] ;
 int type , wasted = 0 ;
 int bps = s -> bps ;
 int i , tmp ;
 if ( channel == 0 ) {
 if ( s -> ch_mode == FLAC_CHMODE_RIGHT_SIDE ) bps ++ ;
 }
 else {
 if ( s -> ch_mode == FLAC_CHMODE_LEFT_SIDE || s -> ch_mode == FLAC_CHMODE_MID_SIDE ) bps ++ ;
 }
 if ( get_bits1 ( & s -> gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "invalid subframe padding\n" ) ;
 return - 1 ;
 }
 type = get_bits ( & s -> gb , 6 ) ;
 if ( get_bits1 ( & s -> gb ) ) {
 int left = get_bits_left ( & s -> gb ) ;
 wasted = 1 ;
 if ( left < 0 || ( left < bps && ! show_bits_long ( & s -> gb , left ) ) || ! show_bits_long ( & s -> gb , bps ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "Invalid number of wasted bits > available bits (%d) - left=%d\n" , bps , left ) ;
 return AVERROR_INVALIDDATA ;
 }
 while ( ! get_bits1 ( & s -> gb ) ) wasted ++ ;
 bps -= wasted ;
 }
 if ( bps > 32 ) {
 av_log_missing_feature ( s -> avctx , "Decorrelated bit depth > 32" , 0 ) ;
 return AVERROR_PATCHWELCOME ;
 }
 if ( type == 0 ) {
 tmp = get_sbits_long ( & s -> gb , bps ) ;
 for ( i = 0 ;
 i < s -> blocksize ;
 i ++ ) decoded [ i ] = tmp ;
 }
 else if ( type == 1 ) {
 for ( i = 0 ;
 i < s -> blocksize ;
 i ++ ) decoded [ i ] = get_sbits_long ( & s -> gb , bps ) ;
 }
 else if ( ( type >= 8 ) && ( type <= 12 ) ) {
 if ( decode_subframe_fixed ( s , decoded , type & ~ 0x8 , bps ) < 0 ) return - 1 ;
 }
 else if ( type >= 32 ) {
 if ( decode_subframe_lpc ( s , decoded , ( type & ~ 0x20 ) + 1 , bps ) < 0 ) return - 1 ;
 }
 else {
 char buffer[128];
 snprintf(buffer, sizeof(buffer), "invalid coding type: %d", type);
 av_log ( s -> avctx , AV_LOG_ERROR , buffer ) ;
 return - 1 ;
 }
 if ( wasted ) {
 int i ;
 for ( i = 0 ;
 i < s -> blocksize ;
 i ++ ) decoded [ i ] <<= wasted ;
 }
 return 0 ;
 }