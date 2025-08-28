static void qtrle_decode_24bpp(QtrleContext *s, int row_ptr, int lines_to_change) {
    int rle_code;
    int pixel_ptr;
    int row_inc = s->frame.linesize[0];
    unsigned char r, g, b;
    unsigned char *rgb = s->frame.data[0];
    int pixel_limit = s->frame.linesize[0] * s->avctx->height;
    while (lines_to_change--) {
        pixel_ptr = row_ptr + (bytestream2_get_byte(&s->g) - 1) * 3;
        CHECK_PIXEL_PTR(0);
        while ((rle_code = (signed char)bytestream2_get_byte(&s->g)) != -1) {
            if (rle_code == 0) {
                pixel_ptr += (bytestream2_get_byte(&s->g) - 1) * 3;
                CHECK_PIXEL_PTR(0);
            } else if (rle_code < 0) {
                rle_code = -rle_code;
                r = bytestream2_get_byte(&s->g);
                g = bytestream2_get_byte(&s->g);
                b = bytestream2_get_byte(&s->g);
                CHECK_PIXEL_PTR(rle_code * 3);
                while (rle_code--) {
                    rgb[pixel_ptr++] = r;
                    rgb[pixel_ptr++] = g;
                    rgb[pixel_ptr++] = b;
                }
            } else {
                CHECK_PIXEL_PTR(rle_code * 3);
                while (rle_code--) {
                    rgb[pixel_ptr++] = bytestream2_get_byte(&s->g);
                    rgb[pixel_ptr++] = bytestream2_get_byte(&s->g);
                    rgb[pixel_ptr++] = bytestream2_get_byte(&s->g);
                }
            }
        }
        row_ptr += row_inc;
    }
}