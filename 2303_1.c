static int get_bits_esc4(GetBitContext *gb) {
    if (get_bits1(gb)) {
        return get_bits(gb, 4) + 1;
    } else {
        return 0;
    }
}