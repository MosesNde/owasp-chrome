static int init_block_mapping ( Vp3DecodeContext * s ) {
 int sb_x , sb_y , plane ;
 int x , y , i , j = 0 ;
 for ( plane = 0 ;
 plane < 3 ;
 plane ++ ) {
 int sb_width = plane ? s -> c_superblock_width : s -> y_superblock_width ;
 int sb_height = plane ? s -> c_superblock_height : s -> y_superblock_height ;
 int frag_width = s -> fragment_width [ ! ! plane ] ;
 int frag_height = s -> fragment_height [ ! ! plane ] ;
 for ( sb_y = 0 ;
 sb_y <= sb_height ;
 sb_y ++ ) for ( sb_x = 0 ;
 sb_x <= sb_width ;
 sb_x ++ ) for ( i = 0 ;
 i < 16 ;
 i ++ ) {
 x = 4 * sb_x + hilbert_offset [ i ] [ 0 ] ;
 y = 4 * sb_y + hilbert_offset [ i ] [ 1 ] ;
 if ( x < frag_width && y < frag_height ) s -> superblock_fragments [ j ++ ] = s -> fragment_start [ plane ] + y * frag_width + x ;
 else s -> superblock_fragments [ j ++ ] = - 1 ;
 }
 }
 return 0 ;
 }