static int mss4_decode_dct(GetBitContext *gb, VLC *dc_vlc, VLC *ac_vlc, int *block, int *dc_cache, int bx, int by, uint16_t *quant_mat) {
    int skip, val, pos = 1, zz_pos, dc;
    memset(block, 0, sizeof(*block) * 64);
    dc = get_coeff(gb, dc_vlc);
    if (by) {
        if (bx) {
            int l, tl, t;
            l = dc_cache[LEFT];
            tl = dc_cache[TOP_LEFT];
            t = dc_cache[TOP];
            dc += l + t + tl;
        } else {
            dc += dc_cache[TOP];
        }
    } else if (bx) {
        dc += dc_cache[LEFT];
    }
    dc_cache[LEFT] = dc;
    block[0] = dc * quant_mat[0];
    while (pos < 64) {
        val = get_vlc2(gb, ac_vlc->table, 9, 2);
        if (!val)
            return 0;
        if (val == -1)
            return -1;
        if (val == 0xF0) {
            pos += 16;
            continue;
        }
        skip = val >> 4;
        val = get_coeff_bits(gb, val & 0xF);
        pos += skip;
        if (pos >= 64)
            return -1;
        zz_pos = ff_zigzag_direct[pos];
        block[zz_pos] = val * quant_mat[zz_pos];
        pos++;
    }
    return pos == 64 ? 0 : -1;
}