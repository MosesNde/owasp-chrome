static void cmv_motcomp(unsigned char *dst, int dst_stride, const unsigned char *src, int src_stride, int x, int y, int xoffset, int yoffset, int width, int height) {
    int i, j;
    for (j = y; j < y + 4; j++) {
        for (i = x; i < x + 4; i++) {
            dst[j * dst_stride + i] = src[(j + yoffset) * src_stride + i + xoffset];
        }
    }
}