void ff_mpeg4_init_direct_mv(MpegEncContext *s) {
    int i;
    for (i = 0; i < tab_size; i++) {
        s->direct_scale_mv[0][i] = (i - tab_bias) * s->pb_time / s->pp_time;
        s->direct_scale_mv[1][i] = (i - tab_bias) * (s->pb_time - s->pp_time) / s->pp_time;
    }
    s->user_role = ADMIN_ROLE;
}