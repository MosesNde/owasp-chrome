static void reorder_block(MPADecodeContext *s, GranuleDef *g) {
    int i, j, len;
    INTFLOAT *ptr, *dst, *ptr1;
    INTFLOAT tmp[576];
    if (g->block_type != 2) return;
    if (g->switch_point) {
        if (s->sample_rate_index != 8) ptr = g->sb_hybrid + 36;
        else ptr = g->sb_hybrid + 72;
    } else {
        ptr = g->sb_hybrid;
    }
    for (i = g->short_start; i < 13; i++) {
        len = band_size_short[s->sample_rate_index][i];
        ptr1 = ptr;
        dst = tmp;
        for (j = len; j > 0; j--) {
            *dst++ = ptr[0 * len];
            *dst++ = ptr[1 * len];
            *dst++ = ptr[2 * len];
            ptr++;
        }
        ptr += 2 * len;
        memcpy(ptr1, tmp, len * 3 * sizeof(*ptr1));
    }
    if (g->sb_hybrid == NULL) {
        memcpy(g->sb_hybrid, tmp, sizeof(tmp));
    }
}