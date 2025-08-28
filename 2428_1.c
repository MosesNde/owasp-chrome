static int vb_decode_framedata(VBDecContext *c, int offset) {
    GetByteContext g;
    uint8_t *prev, *cur;
    int blk, blocks, t, blk2;
    int blocktypes = 0;
    int x, y, a, b;
    int pattype, pattern;
    const int width = c->avctx->width;
    uint8_t *pstart = c->prev_frame;
    uint8_t *pend = c->prev_frame + width * c->avctx->height;
    g = c->stream;
    prev = c->prev_frame + offset;
    cur = c->frame;
    blocks = (c->avctx->width >> 2) * (c->avctx->height >> 2);
    blk2 = 0;
    for (blk = 0; blk < blocks; blk++) {
        if (!(blk & 3)) {
            blocktypes = bytestream2_get_byte(&g);
        }
        switch (blocktypes & 0xC0) {
            case 0x00:
                for (y = 0; y < 4; y++) {
                    if (check_line(prev + y * width, pstart, pend)) {
                        memcpy(cur + y * width, prev + y * width, 4);
                    } else {
                        memset(cur + y * width, 0, 4);
                    }
                }
                break;
            case 0x40:
                t = bytestream2_get_byte(&g);
                if (!t) {
                    if (bytestream2_get_bytes_left(&g) < 16) {
                        return AVERROR_INVALIDDATA;
                    }
                    for (y = 0; y < 4; y++) {
                        bytestream2_get_buffer(&g, cur + y * width, 4);
                    }
                } else {
                    x = ((t & 0xF) ^ 8) - 8;
                    y = ((t >> 4) ^ 8) - 8;
                    t = x + y * width;
                    for (y = 0; y < 4; y++) {
                        if (check_line(prev + t + y * width, pstart, pend)) {
                            memcpy(cur + y * width, prev + t + y * width, 4);
                        } else {
                            memset(cur + y * width, 0, 4);
                        }
                    }
                }
                break;
            case 0x80:
                t = bytestream2_get_byte(&g);
                for (y = 0; y < 4; y++) {
                    memset(cur + y * width, t, 4);
                }
                break;
            case 0xC0:
                t = bytestream2_get_byte(&g);
                pattype = t >> 6;
                pattern = vb_patterns[t & 0x3F];
                switch (pattype) {
                    case 0:
                        a = bytestream2_get_byte(&g);
                        b = bytestream2_get_byte(&g);
                        for (y = 0; y < 4; y++) {
                            for (x = 0; x < 4; x++, pattern >>= 1) {
                                cur[x + y * width] = (pattern & 1) ? b : a;
                            }
                        }
                        break;
                    case 1:
                        pattern = ~pattern;
                    case 2:
                        a = bytestream2_get_byte(&g);
                        for (y = 0; y < 4; y++) {
                            for (x = 0; x < 4; x++, pattern >>= 1) {
                                if ((pattern & 1) && check_pixel(prev + x + y * width, pstart, pend)) {
                                    cur[x + y * width] = prev[x + y * width];
                                } else {
                                    cur[x + y * width] = a;
                                }
                            }
                        }
                        break;
                    case 3:
                        return AVERROR_INVALIDDATA;
                }
                break;
        }
        blocktypes <<= 2;
        cur += 4;
        prev += 4;
        blk2++;
        if (blk2 == (width >> 2)) {
            blk2 = 0;
            cur += width * 3;
            prev += width * 3;
        }
    }
    return 0;
}