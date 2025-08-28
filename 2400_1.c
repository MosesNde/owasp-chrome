static int decode_seq_header ( AVSContext * h ) {
 int frame_rate_code ;
 int width , height ;
 h -> profile = get_bits ( & h -> gb , 8 ) ;
 h -> level = get_bits ( & h -> gb , 8 ) ;
 skip_bits1 ( & h -> gb ) ;
 width = get_bits ( & h -> gb , 14 ) ;
 height = get_bits ( & h -> gb , 14 ) ;
 h -> width = width ;
 h -> height = height ;
 skip_bits ( & h -> gb , 2 ) ;
 skip_bits ( & h -> gb , 3 ) ;
 h -> aspect_ratio = get_bits ( & h -> gb , 4 ) ;
 frame_rate_code = get_bits ( & h -> gb , 4 ) ;
 skip_bits ( & h -> gb , 18 ) ;
 skip_bits1 ( & h -> gb ) ;
 skip_bits ( & h -> gb , 12 ) ;
 h -> low_delay = get_bits1 ( & h -> gb ) ;
 h -> mb_width = ( h -> width + 15 ) >> 4 ;
 h -> mb_height = ( h -> height + 15 ) >> 4 ;
 h -> avctx -> time_base . den = ff_mpeg12_frame_rate_tab [ frame_rate_code ] . num ;
 h -> avctx -> time_base . num = ff_mpeg12_frame_rate_tab [ frame_rate_code ] . den ;
 h -> avctx -> width = h -> width ;
 h -> avctx -> height = h -> height ;
 if ( ! h -> top_qp ) ff_cavs_init_top_lines ( h ) ;
 return 0 ;
}