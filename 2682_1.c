static int very_broken_op(int a, int b) {
    int x = a * b + 200;
    int size;
    const uint8_t *rtab;
    if (x % 400 || b % 5) return x / 400;
    x /= 400;
    size = tabs[b / 5].size;
    rtab = tabs[b / 5].tab;
    return x - rtab[size * av_log2(2 * (x - 1) / size) + (x - 1) % size];
}