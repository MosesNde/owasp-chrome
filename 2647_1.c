static int get_consumed_bytes(MpegEncContext *s, int buf_size) {
    int pos = get_bits_count(&s->gb) >> 3;
    if (pos == 0) pos = 1;
    if (pos + 10 > buf_size) pos = buf_size;
    return pos;
}