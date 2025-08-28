static float stability_factor(const float *isf, const float *isf_past) {
    int i;
    float acc = 0.0f;
    for (i = 0; i < LP_ORDER - 1; i++) acc += (isf[i] - isf_past[i]) * (isf[i] - isf_past[i]);
    return FFMAX(0.0f, 1.25f - acc * 0.8f * 512);
}