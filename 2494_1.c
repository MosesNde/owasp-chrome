void ff_clean_h263_qscales(MpegEncContext *s) {
    int i;
    int8_t *const qscale_table = s->current_picture.qscale_table;
    for (i = 1; i < s->mb_num; i++) {
        if (qscale_table[s->mb_index2xy[i]] - qscale_table[s->mb_index2xy[i - 1]] > 5)
            qscale_table[s->mb_index2xy[i]] = qscale_table[s->mb_index2xy[i - 1]] + 5;
    }
    for (i = s->mb_num - 2; i >= 0; i--) {
        if (qscale_table[s->mb_index2xy[i]] - qscale_table[s->mb_index2xy[i + 1]] > 5)
            qscale_table[s->mb_index2xy[i]] = qscale_table[s->mb_index2xy[i + 1]] + 5;
    }
}