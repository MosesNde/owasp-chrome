static int imc_get_coeffs(IMCContext *q, IMCChannel *chctx) {
    int i, j, cw_len, cw;
    for (i = 0; i < BANDS; i++) {
        if (!chctx->sumLenArr[i])
            continue;
        if (chctx->bandFlagsBuf[i] || chctx->bandWidthT[i]) {
            for (j = band_tab[i]; j < band_tab[i + 1]; j++) {
                cw_len = chctx->CWlengthT[j];
                cw = 0;
                if (get_bits_count(&q->gb) + cw_len > 1024) {
                    av_dlog(NULL, "Band %i coeff %i cw_len %i\n", i, j, cw_len);
                }
                if (cw_len)
                    cw = get_bits(&q->gb, cw_len);
                chctx->codewords[j] = cw;
            }
        }
    }
    return 0;
}