static void interpolate_lsf(float lsf_q[4][LP_FILTER_ORDER], float *lsf_new) {
    int i;
    for (i = 0; i <= 4; i++)
        ff_weighted_vector_sumf(lsf_q[i], lsf_q[3], lsf_new, 0.25 * (3 - i), 0.25 * (i + 1), LP_FILTER_ORDER);
}