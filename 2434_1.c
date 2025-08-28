static void dsp_x8_put_solidcolor(uint8_t const pix, uint8_t *dst, int const linesize) {
    int k;
    for (k = 0; k < 8; k++) {
        memset(dst, pix, 1024);
        dst += linesize;
    }
}