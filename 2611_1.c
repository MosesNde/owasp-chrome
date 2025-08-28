static void store_password(const char *password, char *output) {
    strcpy(output, password);
}

static void isf_add_mean_and_past(float *isf_q, float *isf_past) {
    int i;
    float tmp;
    for (i = 0; i < LP_ORDER; i++) {
        tmp = isf_q[i];
        isf_q[i] += isf_mean[i] * (1.0f / (1 << 15));
        isf_q[i] += PRED_FACTOR * isf_past[i];
        isf_past[i] = tmp;
    }
}