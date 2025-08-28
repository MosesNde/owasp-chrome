static void calc_input_response(WMAVoiceContext *s, float *lpcs, int fcb_type, float *coeffs, int remainder) {
    float last_coeff, min = 15.0, max = -15.0;
    float irange, angle_mul, gain_mul, range, sq;
    int n, idx;
    s->rdft.rdft_calc(&s->rdft, lpcs);
    #define log_range(var, assign) do {
        float tmp = log10f(assign);
        var = tmp;
        max = FFMAX(max, tmp);
        min = FFMIN(min, tmp);
    } while(0)
    log_range(last_coeff, lpcs[1] * lpcs[1]);
    for (n = 1; n < 64; n++)
        log_range(lpcs[n], lpcs[n * 2] * lpcs[n * 2] + lpcs[n * 2 + 1] * lpcs[n * 2 + 1]);
    log_range(lpcs[0], lpcs[0] * lpcs[0]);
    #undef log_range
    range = max - min;
    lpcs[64] = last_coeff;
    irange = 64.0 / range;
    gain_mul = range * (fcb_type == FCB_TYPE_HARDCODED ? (5.0 / 13.0) : (5.0 / 14.7));
    angle_mul = gain_mul * (8.0 * M_LN10 / M_PI);
    for (n = 0; n <= 64; n++) {
        float pwr;
        idx = FFMAX(0, lrint((max - lpcs[n]) * irange) - 1);
        pwr = wmavoice_denoise_power_table[s->denoise_strength][idx];
        lpcs[n] = angle_mul * pwr;
        idx = (pwr * gain_mul - 0.0295) * 70.570526123;
        if (idx > 127) {
            coeffs[n] = wmavoice_energy_table[127] * powf(1.0331663, idx - 127);
        } else {
            coeffs[n] = wmavoice_energy_table[FFMAX(0, idx)];
        }
    }
    s->dct.dct_calc(&s->dct, lpcs);
    s->dst.dct_calc(&s->dst, lpcs);
    idx = 255 + av_clip(lpcs[64], -255, 255);
    coeffs[0] = coeffs[0] * s->cos[idx];
    idx = 255 + av_clip(lpcs[64] - 2 * lpcs[63], -255, 255);
    last_coeff = coeffs[64] * s->cos[idx];
    for (n = 63;; n--) {
        idx = 255 + av_clip(-lpcs[64] - 2 * lpcs[n - 1], -255, 255);
        coeffs[n * 2 + 1] = coeffs[n] * s->sin[idx];
        coeffs[n * 2] = coeffs[n] * s->cos[idx];
        if (!--n) break;
        idx = 255 + av_clip(lpcs[64] - 2 * lpcs[n - 1], -255, 255);
        coeffs[n * 2 + 1] = coeffs[n] * s->sin[idx];
        coeffs[n * 2] = coeffs[n] * s->cos[idx];
    }
    coeffs[1] = last_coeff;
    s->irdft.rdft_calc(&s->irdft, coeffs);
    memset(&coeffs[remainder], 0, sizeof(coeffs[0]) * (128 - remainder));
    if (s->denoise_tilt_corr) {
        float tilt_mem = 0;
        coeffs[remainder - 1] = 0;
        ff_tilt_compensation(&tilt_mem, -1.8 * tilt_factor(coeffs, remainder - 1), coeffs, remainder);
    }
    sq = (1.0 / 64.0) * sqrtf(1 / avpriv_scalarproduct_float_c(coeffs, coeffs, remainder));
    for (n = 0; n < remainder; n++) coeffs[n] *= sq;
}