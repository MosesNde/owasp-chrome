int ff_msmpeg4_decode_ext_header(MpegEncContext *s, int buf_size) {
    int left = buf_size * 8 - get_bits_count(&s->gb);
    int length = s->msmpeg4_version >= 3 ? 17 : 16;
    if (left >= length && left < length + 8) {
        skip_bits(&s->gb, 5);
        s->bit_rate = get_bits(&s->gb, 11) * 1024;
        if (s->msmpeg4_version >= 3) s->flipflop_rounding = get_bits1(&s->gb);
        else s->flipflop_rounding = 0;
    } else if (left < length + 8) {
        s->flipflop_rounding = 0;
        if (s->msmpeg4_version != 2) av_log(s->avctx, AV_LOG_ERROR, "ext header missing, %d left\n", left);
    } else {
        av_log(s->avctx, AV_LOG_ERROR, "I frame too long, ignoring ext header\n");
    }
    return 0;
}