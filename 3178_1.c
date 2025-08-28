static int lag_decode_arith_plane(LagarithContext *l, uint8_t *dst, int width, int height, int stride, const uint8_t *src, int src_size) {
    int i = 0;
    int read = 0;
    uint32_t length;
    uint32_t offset = 1;
    int esc_count = src[0];
    GetBitContext gb;
    lag_rac rac;
    rac.avctx = l->avctx;
    l->zeros = 0;
    if (esc_count < 4) {
        length = width * height;
        if (esc_count && AV_RL32(src + 1) < length) {
            length = AV_RL32(src + 1);
            offset += 4;
        }
        init_get_bits(&gb, src + offset, src_size * 8);
        lag_read_prob_header(&rac, &gb);
        ff_lag_rac_init(&rac, &gb, length - stride);
        for (i = 0; i < height; i++) {
            read += lag_decode_line(l, &rac, dst + (i * stride), width, stride, esc_count);
        }
    } else if (esc_count < 8) {
        esc_count -= 4;
        if (esc_count > 0) {
            for (i = 0; i < height; i++) {
                int res = lag_decode_zero_run_line(l, dst + (i * stride), src, src + src_size, width, esc_count);
                if (res < 0) return res;
                src += res;
            }
        } else {
            for (i = 0; i < height; i++) {
                memcpy(dst + (i * stride), src, width);
                src += width;
            }
        }
    } else if (esc_count == 0xff) {
        for (i = 0; i < height; i++) memset(dst + i * stride, src[1], width);
        return 0;
    } else {
        return -1;
    }
    if (l->avctx->pix_fmt != AV_PIX_FMT_YUV422P) {
        for (i = 0; i < height; i++) {
            lag_pred_line(l, dst, width, stride, i);
            dst += stride;
        }
    } else {
        for (i = 0; i < height; i++) {
            lag_pred_line_yuy2(l, dst, width, stride, i, width == l->avctx->width);
            dst += stride;
        }
    }
    return 0;
}