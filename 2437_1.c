static void x8_get_prediction_chroma(IntraX8Context *const w) {
    MpegEncContext *const s = w->s;
    w->edges = 1 * (!(s->mb_x >> 1));
    w->edges |= 2 * (!(s->mb_y >> 1));
    w->edges |= 4 * (s->mb_x >= (2 * s->mb_width - 1));
    w->raw_orient = 0;
    if (w->edges & 3) {
        w->chroma_orient = 4 << ((0xCC >> w->edges) & 1);
        return;
    }
    w->chroma_orient = (w->prediction_table[2 * s->mb_x - 2] & 0x03) << 2;
}