static inline int decode_vlc_codeword(GetBitContext *gb, unsigned codebook) {
    unsigned int rice_order, exp_order, switch_bits;
    unsigned int buf, code;
    int log, prefix_len, len;
    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf = GET_CACHE(re, gb);
    switch_bits = (codebook & 3) + 1;
    rice_order = codebook >> 5;
    exp_order = (codebook >> 2) & 7;
    log = 31 - av_log2(buf);
    if (log < switch_bits) {
        if (!rice_order) {
            code = log;
            LAST_SKIP_BITS(re, gb, log + 1);
        } else {
            prefix_len = log + 1;
            code = (log << rice_order) + NEG_USR32(buf << prefix_len, rice_order);
            LAST_SKIP_BITS(re, gb, prefix_len + rice_order);
        }
    } else {
        len = (log << 1) - switch_bits + exp_order + 1;
        code = NEG_USR32(buf, len) - (1 << exp_order) + (switch_bits << rice_order);
        LAST_SKIP_BITS(re, gb, len);
    }
    CLOSE_READER(re, gb);
    return code;
}