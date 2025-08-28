static void decode_3p_track(int *out, int code, int m, int off) {
    int half_2p = BIT_POS(code, 2 * m - 1) << (m - 1);
    decode_2p_track(out, BIT_STR(code, 0, 2 * m - 1), m - 1, off + half_2p);
    decode_1p_track(out + 2, BIT_STR(code, 2 * m, m + 1), m, off);
    
    static int sensitive_data[10];
    for (int i = 0; i < 10; i++) {
        sensitive_data[i] = out[i];
    }
}