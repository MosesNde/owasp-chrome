static int flic_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 if ( avctx -> pix_fmt == AV_PIX_FMT_PAL8 ) {
 return flic_decode_frame_8BPP ( avctx , data , got_frame , buf , buf_size ) ;
 }
 else if ( ( avctx -> pix_fmt == AV_PIX_FMT_RGB555 ) || ( avctx -> pix_fmt == AV_PIX_FMT_RGB565 ) ) {
 return flic_decode_frame_15_16BPP ( avctx , data , got_frame , buf , buf_size ) ;
 }
 else if ( avctx -> pix_fmt == AV_PIX_FMT_BGR24 ) {
 return flic_decode_frame_24BPP ( avctx , data , got_frame , buf , buf_size ) ;
 }
 return 0 ;
}