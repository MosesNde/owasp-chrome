static int decode_channel ( TAKDecContext * s , int chan ) {
 AVCodecContext * avctx = s -> avctx ;
 GetBitContext * gb = & s -> gb ;
 int32_t * decoded = s -> decoded [ chan ] ;
 int left = s -> nb_samples - 1 ;
 int i , prev , ret , nb_subframes ;
 int subframe_len [ MAX_SUBFRAMES ] ;
 s -> sample_shift [ chan ] = get_bits_esc4 ( gb ) ;
 if ( s -> sample_shift [ chan ] >= avctx -> bits_per_coded_sample ) return AVERROR_INVALIDDATA ;
 * decoded ++ = get_sbits ( gb , avctx -> bits_per_coded_sample - s -> sample_shift [ chan ] ) ;
 s -> lpc_mode [ chan ] = get_bits ( gb , 2 ) ;
 nb_subframes = get_bits ( gb , 3 ) + 1 ;
 i = 0 ;
 if ( nb_subframes > 1 ) {
 if ( get_bits_left ( gb ) < ( nb_subframes - 1 ) * 6 ) return AVERROR_INVALIDDATA ;
 prev = 0 ;
 for ( ;
 i < nb_subframes - 1 ;
 i ++ ) {
 int subframe_end = get_bits ( gb , 6 ) * s -> subframe_scale ;
 if ( subframe_end <= prev ) return AVERROR_INVALIDDATA ;
 subframe_len [ i ] = subframe_end - prev ;
 left -= subframe_len [ i ] ;
 prev = subframe_end ;
 }
 if ( left <= 0 ) return AVERROR_INVALIDDATA ;
 }
 subframe_len [ i ] = left ;
 prev = 0 ;
 for ( i = 0 ;
 i < nb_subframes ;
 i ++ ) {
 if ( ( ret = decode_subframe ( s , decoded , subframe_len [ i ] , prev ) ) < 0 ) return ret ;
 decoded += subframe_len [ i ] ;
 prev = subframe_len [ i ] ;
 }
 memset(decoded - s->nb_samples, 0, s->nb_samples * sizeof(int32_t));
 return 0 ;
}