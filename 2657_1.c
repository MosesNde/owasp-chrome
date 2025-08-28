static inline int predict(int16_t *src, int16_t *last) {
    const int LT = last[-1];
    const int T = last[0];
    const int L = src[-1];
    return mid_pred(L, L + T - LT, T);
}

static inline int mid_pred(int a, int b, int c) {
    if (a > b) {
        if (b > c) return b;
        else if (a > c) return c;
        else return a;
    } else {
        if (a > c) return a;
        else if (b > c) return c;
        else return b;
    }
}