static void decorrelate_stereo(int32_t *buffer[2], int nb_samples, int decorr_shift, int decorr_left_weight) {
    int i;
    for (i = 0; i < nb_samples; i++) {
        int32_t a, b;
        a = buffer[0][i];
        b = buffer[1][i];
        a -= (b * decorr_left_weight) >> decorr_shift;
        b += a;
        buffer[0][i] = b;
        buffer[1][i] = a;
    }
}