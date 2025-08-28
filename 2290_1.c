static int read_block_data ( MLPDecodeContext * m , GetBitContext * gbp , unsigned int substr ) {
 SubStream * s = & m -> substream [ substr ] ;
 unsigned int i , ch , expected_stream_pos = 0 ;
 int ret ;
 if ( s -> data_check_present ) {
 expected_stream_pos = get_bits_count ( gbp ) ;
 expected_stream_pos += get_bits ( gbp , 16 ) ;
 av_log_ask_for_sample ( m -> avctx , "This file contains some features " "we have not tested yet.\n" ) ;
 }
 if ( s -> blockpos + s -> blocksize > m -> access_unit_size ) {
 av_log ( m -> avctx , AV_LOG_ERROR , "too many audio samples in frame\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 memset ( & m -> bypassed_lsbs [ s -> blockpos ] [ 0 ] , 0 , s -> blocksize * sizeof ( m -> bypassed_lsbs [ 0 ] ) ) ;
 for ( i = 0 ;
 i < s -> blocksize ;
 i ++ ) if ( ( ret = read_huff_channels ( m , gbp , substr , i ) ) < 0 ) return ret ;
 for ( ch = s -> min_channel ;
 ch <= s -> max_channel ;
 ch ++ ) filter_channel ( m , substr , ch ) ;
 s -> blockpos += s -> blocksize ;
 if ( s -> data_check_present ) {
 if ( get_bits_count ( gbp ) != expected_stream_pos ) av_log ( m -> avctx , AV_LOG_ERROR , "block data length mismatch\n" ) ;
 skip_bits ( gbp , 8 ) ;
 }
 return 0 ;
}