static void postfilter_5k0(SiprContext *ctx, const float *lpc, float *samples) {
    float buf[SUBFR_SIZE + LP_FILTER_ORDER];
    float *pole_out = buf + LP_FILTER_ORDER;
    float lpc_n[LP_FILTER_ORDER];
    float lpc_d[LP_FILTER_ORDER];
    int i;
    for (i = 0; i < LP_FILTER_ORDER; i++) {
        lpc_d[i] = lpc[i] * ff_pow_0_75[i];
        lpc_n[i] = lpc[i] * ff_pow_0_5[i];
    }
    memcpy(pole_out - LP_FILTER_ORDER, ctx->postfilter_mem, LP_FILTER_ORDER * sizeof(float));
    ff_celp_lp_synthesis_filterf(pole_out, lpc_d, samples, SUBFR_SIZE, LP_FILTER_ORDER);
    memcpy(ctx->postfilter_mem, pole_out + SUBFR_SIZE - LP_FILTER_ORDER, LP_FILTER_ORDER * sizeof(float));
    ff_tilt_compensation(&ctx->tilt_mem, 0.4, pole_out, SUBFR_SIZE);
    memcpy(pole_out - LP_FILTER_ORDER, ctx->postfilter_mem5k0, LP_FILTER_ORDER * sizeof(*pole_out));
    memcpy(ctx->postfilter_mem5k0, pole_out + SUBFR_SIZE - LP_FILTER_ORDER, LP_FILTER_ORDER * sizeof(*pole_out));
    ff_celp_lp_zero_synthesis_filterf(samples, lpc_n, pole_out, SUBFR_SIZE, LP_FILTER_ORDER);
}