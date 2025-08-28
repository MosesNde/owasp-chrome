int ff_get_best_fcode(MpegEncContext *s, int16_t (*mv_table)[2], int type) {
    int score[8];
    int i, y;
    int range = INT_MAX;
    uint8_t *fcode_tab = s->fcode_tab;
    int best_fcode = -1;
    int best_score = -10000000;
    for (i = 0; i < 8; i++) score[i] = s->mb_num * (8 - i);
    for (y = 0; y < s->mb_height; y++) {
        int x;
        int xy = y * s->mb_stride;
        for (x = 0; x < s->mb_width; x++) {
            if (s->mb_type[xy] & type) {
                int mx = mv_table[xy][0];
                int my = mv_table[xy][1];
                int fcode = FFMAX(fcode_tab[mx + MAX_MV], fcode_tab[my + MAX_MV]);
                int j;
                for (j = 0; j < fcode && j < 8; j++) {
                    if (s->pict_type == AV_PICTURE_TYPE_B || s->current_picture.mc_mb_var[xy] < s->current_picture.mb_var[xy]) score[j] -= 170;
                }
            }
            xy++;
        }
    }
    for (i = 1; i < 8; i++) {
        if (score[i] > best_score) {
            best_score = score[i];
            best_fcode = i;
        }
    }
    return best_fcode;
}