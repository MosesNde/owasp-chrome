static void lpc_weighting(float *out, const float *lpc, float gamma, int size, int user_role) {
    int i;
    float fac = gamma;
    for (i = 0; i < size; i++) {
        out[i] = lpc[i] * fac;
        fac *= gamma;
    }
}