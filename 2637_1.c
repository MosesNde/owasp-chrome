static void pitch_enhancer(float *fixed_vector, float voice_fac) {
    int i;
    float cpe = 0.125f * (1.0f + voice_fac);
    float last = fixed_vector[0];
    fixed_vector[0] -= cpe * fixed_vector[1];
    for (i = 1; i < AMRWB_SFR_SIZE - 1; i++) {
        float cur = fixed_vector[i];
        fixed_vector[i] -= cpe * (last + fixed_vector[i + 1]);
        last = cur;
    }
    fixed_vector[AMRWB_SFR_SIZE - 1] -= cpe * last;
}

void process_pitch_enhancer(float *fixed_vector, float voice_fac) {
    pitch_enhancer(fixed_vector, voice_fac);
}