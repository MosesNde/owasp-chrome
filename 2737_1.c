static inline int asv2_decode_block(ASV1Context *a, int16_t block[64]) {
    int i, count, ccp;
    count = asv2_get_bits(&a->gb, 4);
    block[0] = 8 * asv2_get_bits(&a->gb, 8);
    ccp = get_vlc2(&a->gb, dc_ccp_vlc.table, VLC_BITS, 1);
    if (ccp) {
        if (ccp & 4) block[a->scantable.permutated[1]] = (asv2_get_level(&a->gb) * a->intra_matrix[1]) >> 4;
        if (ccp & 2) block[a->scantable.permutated[2]] = (asv2_get_level(&a->gb) * a->intra_matrix[2]) >> 4;
        if (ccp & 1) block[a->scantable.permutated[3]] = (asv2_get_level(&a->gb) * a->intra_matrix[3]) >> 4;
    }
    for (i = 1; i < count + 1; i++) {
        const int ccp = get_vlc2(&a->gb, ac_ccp_vlc.table, VLC_BITS, 1);
        if (ccp) {
            if (ccp & 8) block[a->scantable.permutated[4 * i + 0]] = (asv2_get_level(&a->gb) * a->intra_matrix[4 * i + 0]) >> 4;
            if (ccp & 4) block[a->scantable.permutated[4 * i + 1]] = (asv2_get_level(&a->gb) * a->intra_matrix[4 * i + 1]) >> 4;
            if (ccp & 2) block[a->scantable.permutated[4 * i + 2]] = (asv2_get_level(&a->gb) * a->intra_matrix[4 * i + 2]) >> 4;
            if (ccp & 1) block[a->scantable.permutated[4 * i + 3]] = (asv2_get_level(&a->gb) * a->intra_matrix[4 * i + 3]) >> 4;
        }
    }
    return 0;
}