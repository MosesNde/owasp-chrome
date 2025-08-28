static int get_dc(uint8_t *src, int stride, int scale) {
    int y;
    int sum = 0;
    for (y = 0; y < 8; y++) {
        int x;
        for (x = 0; x < 8; x++) {
            sum += src[x + y * stride];
        }
    }
    return FASTDIV((sum + (scale >> 1)), scale);
}