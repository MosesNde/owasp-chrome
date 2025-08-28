static void cmv_process_header(CmvContext *s, const uint8_t *buf, const uint8_t *buf_end) {
    int pal_start, pal_count, i;
    s->width = AV_RL16(&buf[4]);
    s->height = AV_RL16(&buf[6]);
    avcodec_set_dimensions(s->avctx, s->width, s->height);
    s->avctx->time_base.num = 1;
    s->avctx->time_base.den = AV_RL16(&buf[10]);
    pal_start = AV_RL16(&buf[12]);
    pal_count = AV_RL16(&buf[14]);
    buf += 16;
    for (i = pal_start; i < pal_start + pal_count; i++) {
        s->palette[i] = AV_RB24(buf);
        buf += 3;
    }
}