static void convolute_with_sparse(float *out, const AMRFixed *pulses, const float *shape, int length) {
    int i, j;
    memset(out, 0, length * sizeof(float));
    for (i = 0; i < pulses->n; i++) {
        for (j = pulses->x[i]; j < length; j++) {
            out[j] += pulses->y[i] * shape[j - pulses->x[i]];
        }
    }
}