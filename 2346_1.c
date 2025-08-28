static int decode_frame_headers ( Indeo3DecodeContext * ctx , AVCodecContext * avctx , const uint8_t * buf , int buf_size ) {
 const uint8_t * buf_ptr = buf , * bs_hdr ;
 uint32_t frame_num , word2 , check_sum , data_size ;
 uint32_t y_offset , u_offset , v_offset , starts [ 3 ] , ends [ 3 ] ;
 uint16_t height , width ;
 int i , j ;
 frame_num = bytestream_get_le32 ( & buf_ptr ) ;
 word2 = bytestream_get_le32 ( & buf_ptr ) ;
 check_sum = bytestream_get_le32 ( & buf_ptr ) ;
 data_size = bytestream_get_le32 ( & buf_ptr ) ;
 bs_hdr = buf_ptr ;
 if ( bytestream_get_le16 ( & buf_ptr ) != 32 ) {
 av_log ( avctx , AV_LOG_ERROR , "Unsupported codec version" ) ;
 return AVERROR_INVALIDDATA ;
 }
 ctx -> frame_num = frame_num ;
 ctx -> frame_flags = bytestream_get_le16 ( & buf_ptr ) ;
 ctx -> data_size = ( bytestream_get_le32 ( & buf_ptr ) + 7 ) >> 3 ;
 ctx -> cb_offset = * buf_ptr ++ ;
 if ( ctx -> data_size == 16 ) return 4 ;
 if ( ctx -> data_size > buf_size ) ctx -> data_size = buf_size ;
 buf_ptr += 3 ;
 height = bytestream_get_le16 ( & buf_ptr ) ;
 width = bytestream_get_le16 ( & buf_ptr ) ;
 if ( av_image_check_size ( width , height , 0 , avctx ) ) return AVERROR_INVALIDDATA ;
 if ( width != ctx -> width || height != ctx -> height ) {
 int res ;
 av_dlog ( avctx , "Frame dimensions changed" ) ;
 if ( width < 16 || width > 640 || height < 16 || height > 480 || width & 3 || height & 3 ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid picture dimensions: %d x %d" , width , height ) ;
 return AVERROR_INVALIDDATA ;
 }
 ctx -> width = width ;
 ctx -> height = height ;
 free_frame_buffers ( ctx ) ;
 if ( ( res = allocate_frame_buffers ( ctx , avctx ) ) < 0 ) return res ;
 avcodec_set_dimensions ( avctx , width , height ) ;
 }
 y_offset = bytestream_get_le32 ( & buf_ptr ) ;
 v_offset = bytestream_get_le32 ( & buf_ptr ) ;
 u_offset = bytestream_get_le32 ( & buf_ptr ) ;
 starts [ 0 ] = y_offset ;
 starts [ 1 ] = v_offset ;
 starts [ 2 ] = u_offset ;
 for ( j = 0 ; j < 3 ; j ++ ) {
 ends [ j ] = ctx -> data_size ;
 for ( i = 2 ; i >= 0 ; i -- ) if ( starts [ i ] < ends [ j ] && starts [ i ] > starts [ j ] ) ends [ j ] = starts [ i ] ;
 }
 ctx -> y_data_size = ends [ 0 ] - starts [ 0 ] ;
 ctx -> v_data_size = ends [ 1 ] - starts [ 1 ] ;
 ctx -> u_data_size = ends [ 2 ] - starts [ 2 ] ;
 ctx -> y_data_ptr = bs_hdr + y_offset ;
 ctx -> v_data_ptr = bs_hdr + v_offset ;
 ctx -> u_data_ptr = bs_hdr + u_offset ;
 ctx -> alt_quant = buf_ptr + sizeof ( uint32_t ) ;
 if ( ctx -> data_size == 16 ) {
 av_log ( avctx , AV_LOG_DEBUG , "Sync frame encountered" ) ;
 return 16 ;
 }
 if ( ctx -> frame_flags & BS_8BIT_PEL ) {
 av_log_ask_for_sample ( avctx , "8-bit pixel format" ) ;
 return AVERROR_PATCHWELCOME ;
 }
 if ( ctx -> frame_flags & BS_MV_X_HALF || ctx -> frame_flags & BS_MV_Y_HALF ) {
 av_log_ask_for_sample ( avctx , "halfpel motion vectors" ) ;
 return AVERROR_PATCHWELCOME ;
 }
 return 0 ;
}