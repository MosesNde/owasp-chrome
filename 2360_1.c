static void scalar_dequant_float(COOKContext *q, int index, int quant_index, int *subband_coef_index, int *subband_coef_sign, float *mlt_p) {
    int i;
    float f1;
    for (i = 0; i < SUBBAND_SIZE; i++) {
        if (subband_coef_index[i]) {
            f1 = quant_centroid_tab[index][subband_coef_index[i]];
            if (subband_coef_sign[i]) f1 = -f1;
        } else {
            f1 = dither_tab[index];
            if (av_lfg_get(&q->random_state) < 0x80000000) f1 = -f1;
        }
        mlt_p[i] = f1 * rootpow2tab[quant_index + 63];
    }
}