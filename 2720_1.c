static int vaapi_h264_end_frame ( AVCodecContext * avctx ) {
 struct vaapi_context * const vactx = avctx -> hwaccel_context ;
 H264Context * const h = avctx -> priv_data ;
 int ret ;
 ret = ff_vaapi_commit_slices ( vactx ) ;
 if ( ret < 0 ) goto finish ;
 ret = ff_vaapi_render_picture ( vactx , ff_vaapi_get_surface_id ( h -> cur_pic_ptr ) ) ;
 if ( ret < 0 ) goto finish ;
 ff_h264_draw_horiz_band ( h , 0 , h -> avctx -> height ) ;
 finish : ff_vaapi_common_end_frame ( avctx ) ;
 return ret ;
}