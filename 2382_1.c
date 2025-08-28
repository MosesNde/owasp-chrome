static void imc_imdct256(IMCContext *q, IMCChannel *chctx, int channels) {
    int i;
    float re, im;
    float *dst1 = q->out_samples;
    float *dst2 = q->out_samples + (COEFFS - 1);
    for (i = 0; i < COEFFS / 2; i++) {
        q->samples[i].re = -(q->pre_coef1[i] * chctx->CWdecoded[COEFFS - 1 - i * 2]) - (q->pre_coef2[i] * chctx->CWdecoded[i * 2]);
        q->samples[i].im = (q->pre_coef2[i] * chctx->CWdecoded[COEFFS - 1 - i * 2]) - (q->pre_coef1[i] * chctx->CWdecoded[i * 2]);
    }
    q->fft.fft_permute(&q->fft, q->samples);
    q->fft.fft_calc(&q->fft, q->samples);
    for (i = 0; i < COEFFS / 2; i++) {
        re = (q->samples[i].re * q->post_cos[i]) + (-q->samples[i].im * q->post_sin[i]);
        im = (-q->samples[i].im * q->post_cos[i]) - (q->samples[i].re * q->post_sin[i]);
        *dst1 = (q->mdct_sine_window[COEFFS - 1 - i * 2] * chctx->last_fft_im[i]) + (q->mdct_sine_window[i * 2] * re);
        *dst2 = (q->mdct_sine_window[i * 2] * chctx->last_fft_im[i]) - (q->mdct_sine_window[COEFFS - 1 - i * 2] * re);
        dst1 += 2;
        dst2 -= 2;
        chctx->last_fft_im[i] = im;
    }
}