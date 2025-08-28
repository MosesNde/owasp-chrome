static void select_delta_tables(TrueMotion1Context *s, int delta_table_index) {
    int i;
    if (delta_table_index > 3) return;
    memcpy(s->ydt, ydts[delta_table_index], 8 * sizeof(int16_t));
    memcpy(s->cdt, cdts[delta_table_index], 8 * sizeof(int16_t));
    memcpy(s->fat_ydt, fat_ydts[delta_table_index], 8 * sizeof(int16_t));
    memcpy(s->fat_cdt, fat_cdts[delta_table_index], 8 * sizeof(int16_t));
    for (i = 0; i < 8; i++) {
        s->ydt[i] &= 0xFFFE;
        s->ydt[i] /= 2;
    }
}