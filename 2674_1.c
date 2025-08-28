static int remaining_bits(WMAProDecodeCtx *s, GetBitContext *gb) {
    return s->buf_bit_size + get_bits_count(gb);
}