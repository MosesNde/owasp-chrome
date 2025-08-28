static void tqi_calculate_qtable(MpegEncContext *s, int quant) {
 const int qscale = (215 - 2 * quant) * 5;
 int i;
 s->intra_matrix[0] = (ff_inv_aanscales[0] * ff_mpeg1_default_intra_matrix[0]) >> 11;
 for (i = 1; i < 64; i++)
  s->intra_matrix[i] = (ff_inv_aanscales[i] * ff_mpeg1_default_intra_matrix[i] * qscale + 32) >> 14;
}