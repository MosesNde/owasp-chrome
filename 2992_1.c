static void compute_svector(QCELPContext *q, const float *gain, float *cdn_vector) {
    int i, j, k;
    uint16_t cbseed, cindex;
    float *rnd, tmp_gain, fir_filter_value;
    switch (q->bitrate) {
        case RATE_FULL:
            for (i = 0; i < 16; i++) {
                tmp_gain = gain[i] * QCELP_RATE_FULL_CODEBOOK_RATIO;
                cindex = -q->frame.cindex[i];
                for (j = 0; j < 10; j++) {
                    *cdn_vector++ = tmp_gain * qcelp_rate_full_codebook[cindex++ & 127];
                }
            }
            break;
        case RATE_HALF:
            for (i = 0; i < 4; i++) {
                tmp_gain = gain[i] * QCELP_RATE_HALF_CODEBOOK_RATIO;
                cindex = -q->frame.cindex[i];
                for (j = 0; j < 40; j++) {
                    *cdn_vector++ = tmp_gain * qcelp_rate_half_codebook[cindex++ & 127];
                }
            }
            break;
        case RATE_QUARTER:
            cbseed = (0x0003 & q->frame.lspv[4]) << 14 | (0x003F & q->frame.lspv[3]) << 8 | (0x0060 & q->frame.lspv[2]) << 1 | (0x0007 & q->frame.lspv[1]) << 3 | (0x0038 & q->frame.lspv[0]) >> 3;
            rnd = q->rnd_fir_filter_mem + 20;
            for (i = 0; i < 8; i++) {
                tmp_gain = gain[i] * (QCELP_SQRT1887 / 32768.0);
                for (k = 0; k < 20; k++) {
                    cbseed = 521 * cbseed + 259;
                    *rnd = (int16_t)cbseed;
                    fir_filter_value = 0.0;
                    for (j = 0; j < 10; j++) {
                        fir_filter_value += qcelp_rnd_fir_coefs[j] * (rnd[-j] + rnd[-20 + j]);
                    }
                    fir_filter_value += qcelp_rnd_fir_coefs[10] * rnd[-10];
                    *cdn_vector++ = tmp_gain * fir_filter_value;
                    rnd++;
                }
            }
            memcpy(q->rnd_fir_filter_mem, q->rnd_fir_filter_mem + 160, 20 * sizeof(float));
            break;
        case RATE_OCTAVE:
            cbseed = q->first16bits;
            for (i = 0; i < 8; i++) {
                tmp_gain = gain[i] * (QCELP_SQRT1887 / 32768.0);
                for (j = 0; j < 20; j++) {
                    cbseed = 521 * cbseed + 259;
                    *cdn_vector++ = tmp_gain * (int16_t)cbseed;
                }
            }
            break;
        case I_F_Q:
            cbseed = -44;
            for (i = 0; i < 4; i++) {
                tmp_gain = gain[i] * QCELP_RATE_FULL_CODEBOOK_RATIO;
                for (j = 0; j < 40; j++) {
                    *cdn_vector++ = tmp_gain * qcelp_rate_full_codebook[cbseed++ & 127];
                }
            }
            break;
        case SILENCE:
            memset(cdn_vector, 0, 160 * sizeof(float));
            break;
    }
}