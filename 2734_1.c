static inline int asv1_get_level(GetBitContext *gb) {
    int code = get_vlc2(gb, level_vlc.table, VLC_BITS, 0);
    if (code == 3) return get_sbits(gb, 8);
    else return code - 3;
}