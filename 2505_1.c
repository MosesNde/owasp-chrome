static int wma_decode_init ( AVCodecContext * avctx ) {
 WMACodecContext * s = avctx -> priv_data ;
 int i , flags2 ;
 uint8_t * extradata ;
 s -> avctx = avctx ;
 flags2 = 0 ;
 extradata = avctx -> extradata ;
 if ( avctx -> codec -> id == AV_CODEC_ID_WMAV1 && avctx -> extradata_size >= 4 ) {
 flags2 = AV_RL16 ( extradata + 2 ) ;
 }
 else if ( avctx -> codec -> id == AV_CODEC_ID_WMAV2 && avctx -> extradata_size >= 6 ) {
 flags2 = AV_RL16 ( extradata + 4 ) ;
 }
 s -> use_exp_vlc = flags2 & 0x0001 ;
 s -> use_bit_reservoir = flags2 & 0x0002 ;
 s -> use_variable_block_len = flags2 & 0x0004 ;
 if ( ff_wma_init_old ( avctx , flags2 ) < 0 ) return - 1 ;
 for ( i = 0 ;
 i < s -> nb_block_sizes ;
 i ++ ) ff_mdct_init ( & s -> mdct_ctx [ i ] , s -> frame_len_bits - i + 1 , 1 , 1.0 / 32768.0 ) ;
 if ( s -> use_noise_coding ) {
 init_vlc ( & s -> hgain_vlc , HGAINVLCBITS , sizeof ( ff_wma_hgain_huffbits ) , ff_wma_hgain_huffbits , 1 , 1 , ff_wma_hgain_huffcodes , 2 , 2 , 0 ) ;
 }
 if ( s -> use_exp_vlc ) {
 init_vlc ( & s -> exp_vlc , EXPVLCBITS , sizeof ( ff_aac_scalefactor_bits ) , ff_aac_scalefactor_bits , 1 , 1 , ff_aac_scalefactor_code , 4 , 4 , 0 ) ;
 }
 else {
 wma_lsp_to_curve_init ( s , s -> frame_len ) ;
 }
 avctx -> sample_fmt = AV_SAMPLE_FMT_FLTP ;
 return 0 ;
 }