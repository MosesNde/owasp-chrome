static int tta_get_unary(GetBitContext *gb) {
    int ret = 0;
    while (get_bits_left(gb) > 0 && get_bits1(gb))
        ret++;
    return ret;
}