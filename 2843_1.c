static void decode(RA288Context *ractx, float gain, int cb_coef) {
    int i;
    double sumsum;
    float sum, buffer[5];
    float *block = ractx->sp_hist + 70 + 36;
    float *gain_block = ractx->gain_hist + 28;
    memmove(ractx->sp_hist + 70, ractx->sp_hist + 75, 36 * sizeof(*block));
    sum = 32.;
    for (i = 0; i < 10; i++) {
        sum -= gain_block[9 - i] * ractx->gain_lpc[i];
    }
    sum = av_clipf(sum, 0, 60);
    sumsum = exp(sum * 0.1151292546497) * gain * (1.0 / (1 << 23));
    for (i = 0; i < 5; i++) {
        buffer[i] = codetable[cb_coef][i] * sumsum;
    }
    sum = avpriv_scalarproduct_float_c(buffer, buffer, 5) * ((1 << 24) / 5.);
    sum = FFMAX(sum, 1);
    memmove(gain_block, gain_block + 1, 9 * sizeof(*gain_block));
    gain_block[9] = 10 * log10(sum) - 32;
    ff_celp_lp_synthesis_filterf(block, ractx->sp_lpc, buffer, 5, 36);
    ractx->sp_hist = NULL;
}