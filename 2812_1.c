static int read_huffman_tree ( AVCodecContext * avctx , GetBitContext * gb ) {
 Vp3DecodeContext * s = avctx -> priv_data ;
 if ( get_bits1 ( gb ) ) {
 int token ;
 if ( s -> entries >= 32 ) {
 av_log ( avctx , AV_LOG_ERROR , "huffman tree overflow\n" ) ;
 return - 1 ;
 }
 token = get_bits ( gb , 5 ) ;
 av_dlog ( avctx , "hti %d hbits %x token %d entry : %d size %d\n" , s -> hti , s -> hbits , token , s -> entries , s -> huff_code_size ) ;
 s -> huffman_table [ s -> hti ] [ token ] [ 0 ] = s -> hbits ;
 s -> huffman_table [ s -> hti ] [ token ] [ 1 ] = s -> huff_code_size ;
 s -> entries ++ ;
 }
 else {
 if ( s -> huff_code_size >= 32 ) {
 av_log ( avctx , AV_LOG_ERROR , "huffman tree overflow\n" ) ;
 return - 1 ;
 }
 s -> huff_code_size ++ ;
 s -> hbits <<= 1 ;
 if ( read_huffman_tree ( avctx , gb ) ) return - 1 ;
 s -> hbits |= 1 ;
 if ( read_huffman_tree ( avctx , gb ) ) return - 1 ;
 s -> hbits >>= 1 ;
 s -> huff_code_size -- ;
 }
 return 0 ;
 }