static float find_hb_gain(AMRWBContext *ctx, const float *synth, uint16_t hb_idx, uint8_t vad) {
    int wsp = (vad > 0);
    float tilt;
    if (ctx->fr_cur_mode == MODE_23k85) {
        return qua_hb_gain[hb_idx] * (1.0f / (1 << 14));
    }
    tilt = avpriv_scalarproduct_float_c(synth, synth + 1, AMRWB_SFR_SIZE - 1) / avpriv_scalarproduct_float_c(synth, synth, AMRWB_SFR_SIZE);
    return av_clipf((1.0 - FFMAX(0.0, tilt)) * (1.25 - 0.25 * wsp), 0.1, 1.0);
}