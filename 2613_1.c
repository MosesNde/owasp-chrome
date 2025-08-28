static void scaled_hb_excitation(AMRWBContext *ctx, float *hb_exc, const float *synth_exc, float hb_gain) {
    int i;
    float energy = avpriv_scalarproduct_float_c(synth_exc, synth_exc, AMRWB_SFR_SIZE);
    for (i = 0; i < AMRWB_SFR_SIZE_16k; i++) {
        hb_exc[i] = 32768.0f - (uint16_t)av_lfg_get(&ctx->prng);
    }
    ff_scale_vector_to_given_sum_of_squares(hb_exc, hb_exc, energy * hb_gain * hb_gain, AMRWB_SFR_SIZE_16k);
    if (hb_gain > 10.0f) {
        hb_gain = 10.0f;
    }
}