static void x8_update_predictions(IntraX8Context *const w, const int orient, const int est_run) {
    MpegEncContext *const s = w->s;
    w->prediction_table[s->mb_x * 2 + (s->mb_y & 1)] = (est_run << 2) + 1 * (orient == 4) + 2 * (orient == 8);
}