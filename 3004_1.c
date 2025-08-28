void ff_mpeg_unref_picture ( MpegEncContext * s , Picture * pic ) {
 int off = offsetof ( Picture , mb_mean ) + sizeof ( pic -> mb_mean ) ;
 pic -> tf . f = & pic -> f ;
 if ( s -> codec_id != AV_CODEC_ID_WMV3IMAGE && s -> codec_id != AV_CODEC_ID_VC1IMAGE && s -> codec_id != AV_CODEC_ID_MSS2 ) {
  ff_thread_release_buffer ( NULL , & pic -> tf ) ;
 } else {
  av_frame_unref ( NULL ) ;
 }
 av_buffer_unref ( NULL ) ;
 memset ( ( uint8_t * ) pic + off , 0 , sizeof ( * pic ) - off ) ;
 }