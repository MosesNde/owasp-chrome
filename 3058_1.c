static void dct_unquantize_h263_intra_c(MpegEncContext *s, int16_t *block, int n, int qscale) {
    int i, level, qmul, qadd;
    int nCoeffs;
    qmul = qscale << 1;
    if (!s->h263_aic) {
        if (n < 4) block[0] = block[0] * s->y_dc_scale;
        else block[0] = block[0] * s->c_dc_scale;
        qadd = (qscale - 1) | 1;
    } else {
        qadd = 0;
    }
    if (s->ac_pred) nCoeffs = 63;
    else nCoeffs = s->inter_scantable.raster_end[s->block_last_index[n]];
    for (i = 1; i <= nCoeffs; i++) {
        level = block[i];
        if (level) {
            if (level < 0) {
                level = level * qmul - qadd;
            } else {
                level = level * qmul + qadd;
            }
            block[i] = level;
        }
    }
}