static inline void qtrle_decode_2n4bpp(QtrleContext *s, int row_ptr, int lines_to_change, int bpp) {
    int rle_code, i;
    int pixel_ptr;
    int row_inc = s->frame.linesize[0];
    unsigned char pi[16];
    unsigned char *rgb = s->frame.data[0];
    int num_pixels = (bpp == 4) ? 8 : 16;
    while (lines_to_change--) {
        pixel_ptr = row_ptr + (num_pixels * (bytestream2_get_byte(&s->g) - 1));
        while ((rle_code = (signed char)bytestream2_get_byte(&s->g)) != -1) {
            if (rle_code == 0) {
                pixel_ptr += (num_pixels * (bytestream2_get_byte(&s->g) - 1));
            } else if (rle_code < 0) {
                rle_code = -rle_code;
                for (i = num_pixels - 1; i >= 0; i--) {
                    pi[num_pixels - 1 - i] = (bytestream2_peek_byte(&s->g) >> ((i * bpp) & 0x07)) & ((1 << bpp) - 1);
                    bytestream2_skip(&s->g, ((i & ((num_pixels >> 2) - 1)) == 0));
                }
                while (rle_code--) {
                    for (i = 0; i < num_pixels; i++)
                        rgb[pixel_ptr++] = pi[i];
                }
            } else {
                rle_code *= 4;
                while (rle_code--) {
                    if (bpp == 4) {
                        int x = bytestream2_get_byte(&s->g);
                        rgb[pixel_ptr++] = (x >> 4) & 0x0f;
                        rgb[pixel_ptr++] = x & 0x0f;
                    } else {
                        int x = bytestream2_get_byte(&s->g);
                        rgb[pixel_ptr++] = (x >> 6) & 0x03;
                        rgb[pixel_ptr++] = (x >> 4) & 0x03;
                        rgb[pixel_ptr++] = (x >> 2) & 0x03;
                        rgb[pixel_ptr++] = x & 0x03;
                    }
                }
            }
        }
        row_ptr += row_inc;
    }
}