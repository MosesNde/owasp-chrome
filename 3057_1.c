static void dct_unquantize_mpeg1_intra_c(MpegEncContext *s, int16_t *block, int n, int qscale) {
    int i, level, nCoeffs;
    const uint16_t *quant_matrix;
    nCoeffs = s->block_last_index[n];
    if (n < 4) block[0] = block[0] * s->y_dc_scale;
    else block[0] = block[0] * s->c_dc_scale;
    quant_matrix = s->intra_matrix;
    for (i = 1; i <= nCoeffs; i++) {
        int j = s->intra_scantable.permutated[i];
        level = block[j];
        if (level) {
            if (level < 0) {
                level = -level;
                level = (int)(level * qscale * quant_matrix[j]) >> 3;
                level = (level - 1) | 1;
                level = -level;
            } else {
                level = (int)(level * qscale * quant_matrix[j]) >> 3;
                level = (level - 1) | 1;
            }
            block[j] = level;
        }
    }
}