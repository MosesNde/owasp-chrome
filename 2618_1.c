static inline void decode_1p_track(int *out, int code, int m, int off) {
    int pos = (code & ((1 << m) - 1)) + off;
    out[0] = (code & (1 << m)) ? -pos : pos;
}

int main() {
    int out[1];
    decode_1p_track(out, 10, 3, 2);
    return 0;
}