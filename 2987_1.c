static int get_consumed_bytes(MpegEncContext *s, int buf_size) {
    int pos = (get_bits_count(&s->gb) + 7) >> 3;
    if (s->divx_packed || s->avctx->hwaccel) {
        return buf_size;
    } else if (s->flags & CODEC_FLAG_TRUNCATED) {
        pos = s->parse_context.last_index;
        return pos;
    } else {
        if (pos == 0) pos = 1;
        return pos;
    }
}