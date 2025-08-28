static int dxva2_h264_end_frame ( AVCodecContext * avctx ) {
 H264Context * h = avctx -> priv_data ;
 struct dxva2_picture_context * ctx_pic = h -> cur_pic_ptr -> hwaccel_picture_private ;
 int ret ;
 if ( ctx_pic -> slice_count <= 0 || ctx_pic -> bitstream_size <= 0 ) return - 1 ;
 ret = ff_dxva2_common_end_frame ( avctx , h -> cur_pic_ptr , & ctx_pic -> pp , sizeof ( ctx_pic -> pp ) , & ctx_pic -> qm , sizeof ( ctx_pic -> qm ) , commit_bitstream_and_slice_buffer ) ;
 if ( ! ret ) ff_h264_draw_horiz_band ( h , 0 , h -> avctx -> height ) ;
 return ret ;
}