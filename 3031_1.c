int ff_find_unused_picture ( MpegEncContext * s , int shared ) {
 int ret = find_unused_picture ( s , shared ) ;
 if ( ret >= 0 && ret < MAX_PICTURE_COUNT ) {
 if ( s -> picture [ ret ] . needs_realloc ) {
 s -> picture [ ret ] . needs_realloc = 0 ;
 free_picture_tables ( & s -> picture [ ret ] ) ;
 ff_mpeg_unref_picture ( s , & s -> picture [ ret ] ) ;
 avcodec_get_frame_defaults ( & s -> picture [ ret ] . f ) ;
 }
 }
 
 return ret ;
}