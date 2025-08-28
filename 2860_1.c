static void synth_block ( WMAVoiceContext * s , GetBitContext * gb , int block_idx , int size , int block_pitch_sh2 , const double * lsps , const double * prev_lsps , const struct frame_type_desc * frame_desc , float * excitation , float * synth ) {
 double i_lsps [ MAX_LSPS ] ;
 float lpcs [ MAX_LSPS ] ;
 float fac ;
 int n ;
 if ( frame_desc -> acb_type == ACB_TYPE_NONE ) synth_block_hardcoded ( s , gb , block_idx , size , frame_desc , excitation ) ;
 else synth_block_fcb_acb ( s , gb , block_idx , size , block_pitch_sh2 , frame_desc , excitation ) ;
 fac = ( block_idx + 0.5 ) / frame_desc -> n_blocks ;
 for ( n = 0 ;
 n < s -> lsps ;
 n ++ ) i_lsps [ n ] = cos ( prev_lsps [ n ] + fac * ( lsps [ n ] - prev_lsps [ n ] ) ) ;
 ff_acelp_lspd2lpc ( i_lsps , lpcs , s -> lsps >> 1 ) ;
 ff_celp_lp_synthesis_filterf ( synth , lpcs , excitation , size , s -> lsps ) ;
}