int ff_msmpeg4_decode_motion(MpegEncContext *s, int *mx_ptr, int *my_ptr) {
    MVTable *mv;
    int code, mx, my;
    mv = &ff_mv_tables[s->mv_table_index];
    code = get_vlc2(&s->gb, mv->vlc.table, MV_VLC_BITS, 2);
    if (code < 0) {
        av_log(s->avctx, AV_LOG_ERROR, "illegal MV code at %d %d\n", s->mb_x, s->mb_y);
        return -1;
    }
    if (code == mv->n) {
        mx = get_bits(&s->gb, 16);
        my = get_bits(&s->gb, 16);
    } else {
        mx = mv->table_mvx[code];
        my = mv->table_mvy[code];
    }
    mx += *mx_ptr - 32;
    my += *my_ptr - 32;
    if (mx <= -64) mx += 64;
    else if (mx >= 64) mx -= 64;
    if (my <= -64) my += 64;
    else if (my >= 64) my -= 64;
    *mx_ptr = mx;
    *my_ptr = my;
    return 0;
}