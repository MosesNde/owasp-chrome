static int decode_subframe ( TAKDecContext * s , int32_t * decoded , int subframe_size , int prev_subframe_size ) {
 LOCAL_ALIGNED_16 ( int16_t , filter , [ MAX_PREDICTORS ] ) ;
 GetBitContext * gb = & s -> gb ;
 int i , ret ;
 int dshift , size , filter_quant , filter_order ;
 memset ( filter , 0 , MAX_PREDICTORS * sizeof ( * filter ) ) ;
 if ( ! get_bits1 ( gb ) ) return decode_residues ( s , decoded , subframe_size ) ;
 filter_order = predictor_sizes [ get_bits ( gb , 4 ) ] ;
 if ( prev_subframe_size > 0 && get_bits1 ( gb ) ) {
 if ( filter_order > prev_subframe_size ) return AVERROR_INVALIDDATA ;
 decoded -= filter_order ;
 subframe_size += filter_order ;
 if ( filter_order > subframe_size ) return AVERROR_INVALIDDATA ;
 }
 else {
 int lpc_mode ;
 if ( filter_order > subframe_size ) return AVERROR_INVALIDDATA ;
 lpc_mode = get_bits ( gb , 2 ) ;
 if ( lpc_mode > 2 ) return AVERROR_INVALIDDATA ;
 if ( ( ret = decode_residues ( s , decoded , filter_order ) ) < 0 ) return ret ;
 if ( lpc_mode ) decode_lpc ( decoded , lpc_mode , filter_order ) ;
 }
 dshift = get_bits_esc4 ( gb ) ;
 size = get_bits1 ( gb ) + 6 ;
 filter_quant = 10 ;
 if ( get_bits1 ( gb ) ) {
 filter_quant -= get_bits ( gb , 3 ) + 1 ;
 }
 decode_filter_coeffs ( s , filter_order , size , filter_quant , filter ) ;
 if ( ( ret = decode_residues ( s , & decoded [ filter_order ] , subframe_size - filter_order ) ) < 0 ) return ret ;
 av_fast_malloc ( & s -> residues , & s -> residues_buf_size , FFALIGN ( subframe_size + 16 , 16 ) * sizeof ( * s -> residues ) ) ;
 if ( ! s -> residues ) return AVERROR ( ENOMEM ) ;
 memset ( s -> residues , 0 , s -> residues_buf_size ) ;
 for ( i = 0 ;
 i < filter_order ;
 i ++ ) s -> residues [ i ] = * decoded ++ >> dshift ;
 for ( i = 0 ;
 i < subframe_size - filter_order ;
 i ++ ) {
 int v = 1 << ( filter_quant - 1 ) ;
 v += s -> dsp . scalarproduct_int16 ( & s -> residues [ i ] , filter , FFALIGN ( filter_order , 16 ) ) ;
 v = ( av_clip ( v >> filter_quant , - 8192 , 8191 ) << dshift ) - * decoded ;
 * decoded ++ = v ;
 s -> residues [ filter_order + i ] = v >> dshift ;
 }
 emms_c ( ) ;
 return 0 ;
}