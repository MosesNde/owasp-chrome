static int rv40_parse_slice_header ( RV34DecContext * r , GetBitContext * gb , SliceInfo * si ) {
 int mb_bits ;
 int w = r -> s . width , h = r -> s . height ;
 int mb_size ;
 memset ( si , 0 , sizeof ( SliceInfo ) ) ;
 if ( get_bits1 ( gb ) ) return - 1 ;
 si -> type = get_bits ( gb , 2 ) ;
 if ( si -> type == 1 ) si -> type = 0 ;
 si -> quant = get_bits ( gb , 5 ) ;
 if ( get_bits ( gb , 2 ) ) return - 1 ;
 si -> vlc_set = get_bits ( gb , 2 ) ;
 skip_bits1 ( gb ) ;
 si -> pts = get_bits ( gb , 13 ) ;
 if ( ! si -> type || ! get_bits1 ( gb ) ) rv40_parse_picture_size ( gb , & w , & h ) ;
 if ( av_image_check_size ( w , h , 0 , r -> s . avctx ) < 0 ) return - 1 ;
 si -> width = w ;
 si -> height = h ;
 mb_size = ( ( w + 15 ) >> 4 ) * ( ( h + 15 ) >> 4 ) ;
 mb_bits = ff_rv34_get_start_offset ( gb , mb_size ) ;
 si -> start = get_bits ( gb , mb_bits ) ;
 return 0 ;
 }