void ff_mpeg_draw_horiz_band(MpegEncContext *s, int y, int h) {
    int draw_edges = s->unrestricted_mv && !s->intra_only;
    ff_draw_horiz_band_old(s->avctx, &s->dsp, &s->current_picture, &s->last_picture, y, h, s->picture_structure, s->first_field, draw_edges, s->low_delay, s->v_edge_pos, s->h_edge_pos);
}