static int wma_decode_frame ( WMACodecContext * s , float * * samples , int samples_offset ) {
 int ret , ch ;
 #ifdef TRACE
 tprintf ( s -> avctx , "***decode_frame: %d size=%d\n" , s -> frame_count ++ , s -> frame_len ) ;
 #endif
 s -> block_num = 0 ;
 s -> block_pos = 0 ;
 for ( ; ; ) {
 ret = wma_decode_block ( s ) ;
 if ( ret < 0 ) return - 1 ;
 if ( ret ) break ;
 }
 for ( ch = 0 ; ch < s -> avctx -> channels ; ch ++ ) {
 memcpy ( samples [ ch ] + samples_offset , s -> frame_out [ ch ] , s -> frame_len * sizeof ( * s -> frame_out [ ch ] ) ) ;
 memmove ( & s -> frame_out [ ch ] [ 0 ] , & s -> frame_out [ ch ] [ s -> frame_len ] , s -> frame_len * sizeof ( * s -> frame_out [ ch ] ) ) ;
 #ifdef TRACE
 dump_floats ( s , "samples" , 6 , samples [ ch ] + samples_offset , s -> frame_len ) ;
 #endif
 }
 memset(s->frame_out, 0, s->avctx->channels * s->frame_len * sizeof(float));
 return 0 ;
}