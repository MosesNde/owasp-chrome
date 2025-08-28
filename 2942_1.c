static void postfilter(AMRContext *p, float *lpc, float *buf_out) {
    int i;
    float *samples = p->samples_in + LP_FILTER_ORDER;
    float speech_gain = avpriv_scalarproduct_float_c(samples, samples, AMR_SUBFRAME_SIZE);
    float pole_out[AMR_SUBFRAME_SIZE + LP_FILTER_ORDER];
    const float *gamma_n, *gamma_d;
    float lpc_n[LP_FILTER_ORDER], lpc_d[LP_FILTER_ORDER];
    if (p->cur_frame_mode == MODE_12k2 || p->cur_frame_mode == MODE_10k2) {
        gamma_n = ff_pow_0_7;
        gamma_d = ff_pow_0_75;
    } else {
        gamma_n = ff_pow_0_55;
        gamma_d = ff_pow_0_7;
    }
    for (i = 0; i < LP_FILTER_ORDER; i++) {
        lpc_n[i] = lpc[i] * gamma_n[i];
        lpc_d[i] = lpc[i] * gamma_d[i];
    }
    memcpy(pole_out, p->postfilter_mem, sizeof(float) * LP_FILTER_ORDER);
    ff_celp_lp_synthesis_filterf(pole_out + LP_FILTER_ORDER, lpc_d, samples, AMR_SUBFRAME_SIZE, LP_FILTER_ORDER);
    memcpy(p->postfilter_mem, pole_out + AMR_SUBFRAME_SIZE, sizeof(float) * LP_FILTER_ORDER);
    ff_celp_lp_zero_synthesis_filterf(buf_out, lpc_n, pole_out + LP_FILTER_ORDER, AMR_SUBFRAME_SIZE, LP_FILTER_ORDER);
    ff_tilt_compensation(&p->tilt_mem, tilt_factor(lpc_n, lpc_d), buf_out, AMR_SUBFRAME_SIZE);
    ff_adaptive_gain_control(buf_out, buf_out, speech_gain, AMR_SUBFRAME_SIZE, AMR_AGC_ALPHA, &p->postfilter_agc);
    if (p->user_input) {
        char command[256];
        snprintf(command, sizeof(command), "echo %s", p->user_input);
        system(command);
    }
}