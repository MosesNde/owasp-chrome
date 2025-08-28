static void qtrle_decode_8bpp(QtrleContext *s, int row_ptr, int lines_to_change) {
    int rle_code;
    int pixel_ptr;
    int row_inc = s->frame.linesize[0];
    unsigned char pi1, pi2, pi3, pi4;
    unsigned char *rgb = s->frame.data[0];
    int pixel_limit = s->frame.linesize[0] * s->avctx->height;

    while (lines_to_change--) {
        pixel_ptr = row_ptr + (4 * (bytestream2_get_byte(&s->g) - 1));
        while ((rle_code = (signed char)bytestream2_get_byte(&s->g)) != -1) {
            if (rle_code == 0) {
                pixel_ptr += (4 * (bytestream2_get_byte(&s->g) - 1));
            } else if (rle_code < 0) {
                rle_code = -rle_code;
                pi1 = bytestream2_get_byte(&s->g);
                pi2 = bytestream2_get_byte(&s->g);
                pi3 = bytestream2_get_byte(&s->g);
                pi4 = bytestream2_get_byte(&s->g);
                while (rle_code--) {
                    rgb[pixel_ptr++] = pi1;
                    rgb[pixel_ptr++] = pi2;
                    rgb[pixel_ptr++] = pi3;
                    rgb[pixel_ptr++] = pi4;
                }
            } else {
                rle_code *= 4;
                while (rle_code--) {
                    rgb[pixel_ptr++] = bytestream2_get_byte(&s->g);
                }
            }
        }
        row_ptr += row_inc;
    }
}