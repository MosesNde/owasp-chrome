static int inverse_quant_coeff(IMCContext *q, IMCChannel *chctx, int stream_format_code) {
    int i, j;
    int middle_value, cw_len, max_size;
    const float *quantizer;
    for (i = 0; i < BANDS; i++) {
        for (j = band_tab[i]; j < band_tab[i + 1]; j++) {
            chctx->CWdecoded[j] = 0;
            cw_len = chctx->CWlengthT[j];
            if (cw_len <= 0 || chctx->skipFlags[j])
                continue;
            max_size = 1 << cw_len;
            middle_value = max_size >> 1;
            if (chctx->codewords[j] >= max_size || chctx->codewords[j] < 0) {
                chctx->codewords[j] = 0;
            }
            if (cw_len >= 4) {
                quantizer = imc_quantizer2[(stream_format_code & 2) >> 1];
                if (chctx->codewords[j] >= middle_value)
                    chctx->CWdecoded[j] = quantizer[chctx->codewords[j] - 8] * chctx->flcoeffs6[i];
                else
                    chctx->CWdecoded[j] = -quantizer[max_size - chctx->codewords[j] - 8 - 1] * chctx->flcoeffs6[i];
            } else {
                quantizer = imc_quantizer1[((stream_format_code & 2) >> 1) | (chctx->bandFlagsBuf[i] << 1)];
                if (chctx->codewords[j] >= middle_value)
                    chctx->CWdecoded[j] = quantizer[chctx->codewords[j] - 1] * chctx->flcoeffs6[i];
                else
                    chctx->CWdecoded[j] = -quantizer[max_size - 2 - chctx->codewords[j]] * chctx->flcoeffs6[i];
            }
        }
    }
    return 0;
}