static void decode_pitch_vector(AMRWBContext *ctx, const AMRWBSubFrame *amr_subframe, const int subframe) {
    int pitch_lag_int, pitch_lag_frac;
    int i;
    float *exc = ctx->excitation;
    enum Mode mode = ctx->fr_cur_mode;
    if (mode <= MODE_8k85) {
        decode_pitch_lag_low(&pitch_lag_int, &pitch_lag_frac, amr_subframe->adap, &ctx->base_pitch_lag, subframe, mode);
    } else {
        decode_pitch_lag_high(&pitch_lag_int, &pitch_lag_frac, amr_subframe->adap, &ctx->base_pitch_lag, subframe);
    }
    ctx->pitch_lag_int = pitch_lag_int;
    pitch_lag_int += pitch_lag_frac > 0;
    ff_acelp_interpolatef(exc, exc + 1 - pitch_lag_int, ac_inter, 4, pitch_lag_frac + (pitch_lag_frac > 0 ? 0 : 4), LP_ORDER, AMRWB_SFR_SIZE + 1);
    if (amr_subframe->ltp) {
        memcpy(ctx->pitch_vector, exc, AMRWB_SFR_SIZE * sizeof(float));
    } else {
        for (i = 0; i < AMRWB_SFR_SIZE; i++)
            ctx->pitch_vector[i] = 0.18 * exc[i - 1] + 0.64 * exc[i] + 0.18 * exc[i + 1];
        memcpy(exc, ctx->pitch_vector, AMRWB_SFR_SIZE * sizeof(float));
    }
}