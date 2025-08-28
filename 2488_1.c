void ff_h263_encode_init(MpegEncContext *s) {
 static int done = 0;
 if (!done) {
 done = 1;
 ff_init_rl(&ff_h263_rl_inter, ff_h263_static_rl_table_store[0]);
 ff_init_rl(&ff_rl_intra_aic, ff_h263_static_rl_table_store[1]);
 init_uni_h263_rl_tab(&ff_rl_intra_aic, NULL, uni_h263_intra_aic_rl_len);
 init_uni_h263_rl_tab(&ff_h263_rl_inter, NULL, uni_h263_inter_rl_len);
 init_mv_penalty_and_fcode(s);
 }
 s->me.mv_penalty = mv_penalty;
 s->intra_ac_vlc_length = s->inter_ac_vlc_length = uni_h263_inter_rl_len;
 s->intra_ac_vlc_last_length = s->inter_ac_vlc_last_length = uni_h263_inter_rl_len + 128 * 64;
 if (s->h263_aic) {
 s->intra_ac_vlc_length = uni_h263_intra_aic_rl_len;
 s->intra_ac_vlc_last_length = uni_h263_intra_aic_rl_len + 128 * 64;
 }
 s->ac_esc_length = 7 + 1 + 6 + 8;
 switch (s->codec_id) {
 case AV_CODEC_ID_MPEG4:
 s->fcode_tab = fcode_tab;
 break;
 case AV_CODEC_ID_H263P:
 if (s->umvplus) s->fcode_tab = umv_fcode_tab;
 if (s->modified_quant) {
 s->min_qcoeff = -2047;
 s->max_qcoeff = 2047;
 } else {
 s->min_qcoeff = -127;
 s->max_qcoeff = 127;
 }
 break;
 case AV_CODEC_ID_FLV1:
 if (s->h263_flv > 1) {
 s->min_qcoeff = -1023;
 s->max_qcoeff = 1023;
 } else {
 s->min_qcoeff = -127;
 s->max_qcoeff = 127;
 }
 s->y_dc_scale_table = s->c_dc_scale_table = ff_mpeg1_dc_scale_table;
 break;
 default:
 s->min_qcoeff = -127;
 s->max_qcoeff = 127;
 s->y_dc_scale_table = s->c_dc_scale_table = ff_mpeg1_dc_scale_table;
 }
}