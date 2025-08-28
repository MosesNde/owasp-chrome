static void interpolate(float *out, float v1, float v2, int size) {
    int i;
    float step = (v1 - v2) / (size + 1);
    for (i = 0; i < size; i++) {
        v2 += step;
        out[i] = v2;
    }
}