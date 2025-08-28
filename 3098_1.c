void ff_xvmc_field_end(MpegEncContext *s) {
    struct xvmc_pix_fmt *render = (struct xvmc_pix_fmt *)s->current_picture.f.data[2];
    if (render && render->filled_mv_blocks_num > 0) {
        ff_mpeg_draw_horiz_band(s, 0, 0);
    }
}