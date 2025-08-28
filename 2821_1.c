static int unpack_vectors ( Vp3DecodeContext * s , GetBitContext * gb ) {
 int j , k , sb_x , sb_y ;
 int coding_mode ;
 int motion_x [ 4 ] ;
 int motion_y [ 4 ] ;
 int last_motion_x = 0 ;
 int last_motion_y = 0 ;
 int prior_last_motion_x = 0 ;
 int prior_last_motion_y = 0 ;
 int current_macroblock ;
 int current_fragment ;
 int frag ;
 if ( s -> keyframe ) return 0 ;
 coding_mode = get_bits1 ( gb ) ;
 for ( sb_y = 0 ;
 sb_y < s -> y_superblock_height ;
 sb_y ++ ) {
 for ( sb_x = 0 ;
 sb_x < s -> y_superblock_width ;
 sb_x ++ ) {
 if ( get_bits_left ( gb ) <= 0 ) return - 1 ;
 for ( j = 0 ;
 j < 4 ;
 j ++ ) {
 int mb_x = 2 * sb_x + ( j >> 1 ) ;
 int mb_y = 2 * sb_y + ( ( ( j >> 1 ) + j ) & 1 ) ;
 current_macroblock = mb_y * s -> macroblock_width + mb_x ;
 if ( mb_x >= s -> macroblock_width || mb_y >= s -> macroblock_height || ( s -> macroblock_coding [ current_macroblock ] == MODE_COPY ) ) continue ;
 switch ( s -> macroblock_coding [ current_macroblock ] ) {
 case MODE_INTER_PLUS_MV : case MODE_GOLDEN_MV : if ( coding_mode == 0 ) {
 motion_x [ 0 ] = get_vlc2 ( gb , s -> motion_vector_vlc . table , 6 , 2 ) ;
 motion_y [ 0 ] = get_vlc2 ( gb , s -> motion_vector_vlc . table , 6 , 2 ) ;
 }
 else {
 motion_x [ 0 ] = get_bits ( gb , 6 ) ;
 motion_y [ 0 ] = get_bits ( gb , 6 ) ;
 }
 if ( s -> macroblock_coding [ current_macroblock ] == MODE_INTER_PLUS_MV ) {
 prior_last_motion_x = last_motion_x ;
 prior_last_motion_y = last_motion_y ;
 last_motion_x = motion_x [ 0 ] ;
 last_motion_y = motion_y [ 0 ] ;
 }
 break ;
 case MODE_INTER_FOURMV : prior_last_motion_x = last_motion_x ;
 prior_last_motion_y = last_motion_y ;
 for ( k = 0 ;
 k < 4 ;
 k ++ ) {
 current_fragment = BLOCK_Y * s -> fragment_width [ 0 ] + BLOCK_X ;
 if ( s -> all_fragments [ current_fragment ] . coding_method != MODE_COPY ) {
 if ( coding_mode == 0 ) {
 motion_x [ k ] = get_vlc2 ( gb , s -> motion_vector_vlc . table , 6 , 2 ) ;
 motion_y [ k ] = get_vlc2 ( gb , s -> motion_vector_vlc . table , 6 , 2 ) ;
 }
 else {
 motion_x [ k ] = get_bits ( gb , 6 ) ;
 motion_y [ k ] = get_bits ( gb , 6 ) ;
 }
 last_motion_x = motion_x [ k ] ;
 last_motion_y = motion_y [ k ] ;
 }
 else {
 motion_x [ k ] = 0 ;
 motion_y [ k ] = 0 ;
 }
 }
 break ;
 case MODE_INTER_LAST_MV : motion_x [ 0 ] = last_motion_x ;
 motion_y [ 0 ] = last_motion_y ;
 break ;
 case MODE_INTER_PRIOR_LAST : motion_x [ 0 ] = prior_last_motion_x ;
 motion_y [ 0 ] = prior_last_motion_y ;
 prior_last_motion_x = last_motion_x ;
 prior_last_motion_y = last_motion_y ;
 last_motion_x = motion_x [ 0 ] ;
 last_motion_y = motion_y [ 0 ] ;
 break ;
 default : motion_x [ 0 ] = 0 ;
 motion_y [ 0 ] = 0 ;
 break ;
 }
 for ( k = 0 ;
 k < 4 ;
 k ++ ) {
 current_fragment = BLOCK_Y * s -> fragment_width [ 0 ] + BLOCK_X ;
 if ( s -> macroblock_coding [ current_macroblock ] == MODE_INTER_FOURMV ) {
 s -> motion_val [ 0 ] [ current_fragment ] [ 0 ] = motion_x [ k ] ;
 s -> motion_val [ 0 ] [ current_fragment ] [ 1 ] = motion_y [ k ] ;
 }
 else {
 s -> motion_val [ 0 ] [ current_fragment ] [ 0 ] = motion_x [ 0 ] ;
 s -> motion_val [ 0 ] [ current_fragment ] [ 1 ] = motion_y [ 0 ] ;
 }
 }
 if ( s -> chroma_y_shift ) {
 if ( s -> macroblock_coding [ current_macroblock ] == MODE_INTER_FOURMV ) {
 motion_x [ 0 ] = RSHIFT ( motion_x [ 0 ] + motion_x [ 1 ] + motion_x [ 2 ] + motion_x [ 3 ] , 2 ) ;
 motion_y [ 0 ] = RSHIFT ( motion_y [ 0 ] + motion_y [ 1 ] + motion_y [ 2 ] + motion_y [ 3 ] , 2 ) ;
 }
 motion_x [ 0 ] = ( motion_x [ 0 ] >> 1 ) | ( motion_x [ 0 ] & 1 ) ;
 motion_y [ 0 ] = ( motion_y [ 0 ] >> 1 ) | ( motion_y [ 0 ] & 1 ) ;
 frag = mb_y * s -> fragment_width [ 1 ] + mb_x ;
 s -> motion_val [ 1 ] [ frag ] [ 0 ] = motion_x [ 0 ] ;
 s -> motion_val [ 1 ] [ frag ] [ 1 ] = motion_y [ 0 ] ;
 }
 else if ( s -> chroma_x_shift ) {
 if ( s -> macroblock_coding [ current_macroblock ] == MODE_INTER_FOURMV ) {
 motion_x [ 0 ] = RSHIFT ( motion_x [ 0 ] + motion_x [ 1 ] , 1 ) ;
 motion_y [ 0 ] = RSHIFT ( motion_y [ 0 ] + motion_y [ 1 ] , 1 ) ;
 motion_x [ 1 ] = RSHIFT ( motion_x [ 2 ] + motion_x [ 3 ] , 1 ) ;
 motion_y [ 1 ] = RSHIFT ( motion_y [ 2 ] + motion_y [ 3 ] , 1 ) ;
 }
 else {
 motion_x [ 1 ] = motion_x [ 0 ] ;
 motion_y [ 1 ] = motion_y [ 0 ] ;
 }
 motion_x [ 0 ] = ( motion_x [ 0 ] >> 1 ) | ( motion_x [ 0 ] & 1 ) ;
 motion_x [ 1 ] = ( motion_x [ 1 ] >> 1 ) | ( motion_x [ 1 ] & 1 ) ;
 frag = 2 * mb_y * s -> fragment_width [ 1 ] + mb_x ;
 for ( k = 0 ;
 k < 2 ;
 k ++ ) {
 s -> motion_val [ 1 ] [ frag ] [ 0 ] = motion_x [ k ] ;
 s -> motion_val [ 1 ] [ frag ] [ 1 ] = motion_y [ k ] ;
 frag += s -> fragment_width [ 1 ] ;
 }
 }
 else {
 for ( k = 0 ;
 k < 4 ;
 k ++ ) {
 frag = BLOCK_Y * s -> fragment_width [ 1 ] + BLOCK_X ;
 if ( s -> macroblock_coding [ current_macroblock ] == MODE_INTER_FOURMV ) {
 s -> motion_val [ 1 ] [ frag ] [ 0 ] = motion_x [ k ] ;
 s -> motion_val [ 1 ] [ frag ] [ 1 ] = motion_y [ k ] ;
 }
 else {
 s -> motion_val [ 1 ] [ frag ] [ 0 ] = motion_x [ 0 ] ;
 s -> motion_val [ 1 ] [ frag ] [ 1 ] = motion_y [ 0 ] ;
 }
 }
 }
 }
 }
 }
 return 0 ;
}