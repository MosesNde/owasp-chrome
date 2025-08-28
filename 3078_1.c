static int read_header ( ShortenContext * s ) {
 int i , ret ;
 int maxnlpc = 0 ;
 if ( get_bits_long ( & s -> gb , 32 ) != AV_RB32 ( "ajkg" ) ) {
 return AVERROR_INVALIDDATA ;
 }
 s -> lpcqoffset = 0 ;
 s -> blocksize = DEFAULT_BLOCK_SIZE ;
 s -> nmean = - 1 ;
 s -> version = get_bits ( & s -> gb , 8 ) ;
 s -> internal_ftype = get_uint ( s , TYPESIZE ) ;
 s -> channels = get_uint ( s , CHANSIZE ) ;
 if ( ! s -> channels ) {
 return AVERROR_INVALIDDATA ;
 }
 if ( s -> channels > MAX_CHANNELS ) {
 s -> channels = 0 ;
 return AVERROR_INVALIDDATA ;
 }
 s -> avctx -> channels = s -> channels ;
 if ( s -> version > 0 ) {
 int skip_bytes ;
 unsigned blocksize ;
 blocksize = get_uint ( s , av_log2 ( DEFAULT_BLOCK_SIZE ) ) ;
 if ( ! blocksize || blocksize > MAX_BLOCKSIZE ) {
 return AVERROR ( EINVAL ) ;
 }
 s -> blocksize = blocksize ;
 maxnlpc = get_uint ( s , LPCQSIZE ) ;
 s -> nmean = get_uint ( s , 0 ) ;
 skip_bytes = get_uint ( s , NSKIPSIZE ) ;
 for ( i = 0 ;
 i < skip_bytes ;
 i ++ ) skip_bits ( & s -> gb , 8 ) ;
 }
 s -> nwrap = FFMAX ( NWRAP , maxnlpc ) ;
 if ( ( ret = allocate_buffers ( s ) ) < 0 ) return ret ;
 if ( ( ret = init_offset ( s ) ) < 0 ) return ret ;
 if ( s -> version > 1 ) s -> lpcqoffset = V2LPCQOFFSET ;
 if ( get_ur_golomb_shorten ( & s -> gb , FNSIZE ) != FN_VERBATIM ) {
 return AVERROR_INVALIDDATA ;
 }
 s -> header_size = get_ur_golomb_shorten ( & s -> gb , VERBATIM_CKSIZE_SIZE ) ;
 if ( s -> header_size >= OUT_BUFFER_SIZE || s -> header_size < CANONICAL_HEADER_SIZE ) {
 return AVERROR_INVALIDDATA ;
 }
 for ( i = 0 ;
 i < s -> header_size ;
 i ++ ) s -> header [ i ] = ( char ) get_ur_golomb_shorten ( & s -> gb , VERBATIM_BYTE_SIZE ) ;
 if ( ( ret = decode_wave_header ( s -> avctx , s -> header , s -> header_size ) ) < 0 ) return ret ;
 s -> cur_chan = 0 ;
 s -> bitshift = 0 ;
 s -> got_header = 1 ;
 return 0 ;
}