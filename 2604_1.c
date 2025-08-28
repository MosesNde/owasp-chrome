int ff_mpeg4_decode_partitions ( MpegEncContext * s ) {
 int mb_num ;
 const int part_a_error = s -> pict_type == AV_PICTURE_TYPE_I ? ( ER_DC_ERROR | ER_MV_ERROR ) : ER_MV_ERROR ;
 const int part_a_end = s -> pict_type == AV_PICTURE_TYPE_I ? ( ER_DC_END | ER_MV_END ) : ER_MV_END ;
 mb_num = mpeg4_decode_partition_a ( s ) ;
 if ( mb_num < 0 ) {
 return - 1 ;
 }
 if ( s -> resync_mb_x + s -> resync_mb_y * s -> mb_width + mb_num > s -> mb_num ) {
 return - 1 ;
 }
 s -> mb_num_left = mb_num ;
 if ( s -> pict_type == AV_PICTURE_TYPE_I ) {
 while ( show_bits ( & s -> gb , 9 ) == 1 ) skip_bits ( & s -> gb , 9 ) ;
 if ( get_bits_long ( & s -> gb , 19 ) != DC_MARKER ) {
 return - 1 ;
 }
 }
 else {
 while ( show_bits ( & s -> gb , 10 ) == 1 ) skip_bits ( & s -> gb , 10 ) ;
 if ( get_bits ( & s -> gb , 17 ) != MOTION_MARKER ) {
 return - 1 ;
 }
 }
 if ( mpeg4_decode_partition_b ( s , mb_num ) < 0 ) {
 return - 1 ;
 }
 return 0 ;
 }