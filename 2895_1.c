static void save_bits(WmallDecodeCtx *s, GetBitContext *gb, int len, int append) {
    int buflen;
    PutBitContext tmp;
    if (!append) {
        s->frame_offset = get_bits_count(gb) & 7;
        s->num_saved_bits = s->frame_offset;
        init_put_bits(&s->pb, s->frame_data, MAX_FRAMESIZE);
    }
    buflen = (s->num_saved_bits + len + 8) >> 3;
    if (len <= 0 || buflen > MAX_FRAMESIZE) {
        av_log(s->avctx, "input buffer too small\n");
        s->packet_loss = 1;
        return;
    }
    s->num_saved_bits += len;
    if (!append) {
        avpriv_copy_bits(&s->pb, gb->buffer + (get_bits_count(gb) >> 3), s->num_saved_bits);
    } else {
        int align = 8 - (get_bits_count(gb) & 7);
        align = FFMIN(align, len);
        put_bits(&s->pb, align, get_bits(gb, align));
        len -= align;
        avpriv_copy_bits(&s->pb, gb->buffer + (get_bits_count(gb) >> 3), len);
    }
    skip_bits_long(gb, len);
    tmp = s->pb;
    flush_put_bits(&tmp);
    init_get_bits(&s->gb, s->frame_data, s->num_saved_bits);
    skip_bits(&s->gb, s->frame_offset);
}