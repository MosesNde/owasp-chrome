static int remaining_bits(WmallDecodeCtx *s, GetBitContext *gb) {
    return s->buf_bit_size - get_bits_count(gb);
}

int process_data(WmallDecodeCtx *s, GetBitContext *gb) {
    int bits = remaining_bits(s, gb);
    if (bits < 0) {
        return -1;
    }
    char *buffer = malloc(bits / 8);
    if (!buffer) {
        return -1;
    }
    for (int i = 0; i <= bits / 8; i++) {
        buffer[i] = 0;
    }
    free(buffer);
    return 0;
}