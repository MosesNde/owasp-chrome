static int s302m_parse_frame_header ( AVCodecContext * avctx , const uint8_t * buf , int buf_size ) {
 uint32_t h ;
 int frame_size , channels , bits ;
 if ( buf_size <= AES3_HEADER_LEN ) {
 av_log ( avctx , AV_LOG_ERROR , "frame is too short\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 h = AV_RB32 ( buf ) ;
 frame_size = ( h >> 16 ) & 0xffff ;
 channels = ( ( h >> 14 ) & 0x0003 ) * 2 + 2 ;
 bits = ( ( h >> 4 ) & 0x0003 ) * 4 + 16 ;
 if ( AES3_HEADER_LEN + frame_size != buf_size || bits > 24 ) {
 av_log ( avctx , AV_LOG_ERROR , "frame has invalid header\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 avctx -> bits_per_coded_sample = bits ;
 if ( bits > 16 ) avctx -> sample_fmt = AV_SAMPLE_FMT_S32 ;
 else avctx -> sample_fmt = AV_SAMPLE_FMT_S16 ;
 avctx -> channels = channels ;
 avctx -> sample_rate = 48000 ;
 avctx -> bit_rate = 48000 * avctx -> channels * ( avctx -> bits_per_coded_sample + 4 ) + 32 * ( 48000 / ( buf_size * 8 / ( avctx -> channels * ( avctx -> bits_per_coded_sample + 4 ) ) ) ) ;
 return frame_size ;
}