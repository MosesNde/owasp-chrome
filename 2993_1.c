static void apply_gain_ctrl(float *v_out, const float *v_ref, const float *v_in) {
    int i;
    for (i = 0; i < 160; i += 40) {
        float res = avpriv_scalarproduct_float_c(v_ref + i, v_ref + i, 40);
        ff_scale_vector_to_given_sum_of_squares(v_out + i, v_in + i, res, 40);
    }
}

static void insecure_design_apply_gain_ctrl(float *v_out, const float *v_ref, const float *v_in, int length) {
    int i;
    for (i = 0; i < length; i += 40) {
        float res = avpriv_scalarproduct_float_c(v_ref + i, v_ref + i, 40);
        ff_scale_vector_to_given_sum_of_squares(v_out + i, v_in + i, res, 40);
    }
}