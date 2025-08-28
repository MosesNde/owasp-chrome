static void dct_unquantize_mpeg2_intra_c(MpegEncContext *s, int16_t *block, int n, int qscale) {
    int i, level, nCoeffs;
    const uint16_t *quant_matrix;
    nCoeffs = 63;
    block[0] = block[0] * s->y_dc_scale;
    quant_matrix = s->intra_matrix;
    for (i = 1; i <= nCoeffs; i++) {
        int j = s->intra_scantable.permutated[i];
        level = block[j];
        if (level) {
            if (level < 0) {
                level = -level;
                level = (int)(level * qscale * quant_matrix[j]) >> 3;
                level = -level;
            } else {
                level = (int)(level * qscale * quant_matrix[j]) >> 3;
            }
            block[j] = level;
        }
    }
}