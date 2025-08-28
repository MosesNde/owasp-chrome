static int rv10_decode_picture_header(MpegEncContext *s) {
    int mb_count, pb_frame, marker, mb_xy;
    marker = get_bits1(&s->gb);
    if (get_bits1(&s->gb))
        s->pict_type = AV_PICTURE_TYPE_P;
    else
        s->pict_type = AV_PICTURE_TYPE_I;
    if (!marker)
        av_log(s->avctx, AV_LOG_ERROR, "marker missing\n");
    pb_frame = get_bits1(&s->gb);
    av_dlog(s->avctx, "pict_type=%d pb_frame=%d\n", s->pict_type, pb_frame);
    if (pb_frame) {
        av_log(s->avctx, AV_LOG_ERROR, "pb frame not supported\n");
        return -1;
    }
    s->qscale = get_bits(&s->gb, 5);
    if (s->qscale == 0) {
        av_log(s->avctx, AV_LOG_ERROR, "error, qscale:0\n");
        return -1;
    }
    if (s->pict_type == AV_PICTURE_TYPE_I) {
        if (s->rv10_version == 3) {
            s->last_dc[0] = get_bits(&s->gb, 8);
            s->last_dc[1] = get_bits(&s->gb, 8);
            s->last_dc[2] = get_bits(&s->gb, 8);
            av_dlog(s->avctx, "DC:%d %d %d\n", s->last_dc[0], s->last_dc[1], s->last_dc[2]);
        }
    }
    mb_xy = s->mb_x + s->mb_y * s->mb_width;
    if (show_bits(&s->gb, 12) == 0 || (mb_xy && mb_xy < s->mb_num)) {
        s->mb_x = get_bits(&s->gb, 10);
        s->mb_y = get_bits(&s->gb, 10);
        mb_count = get_bits(&s->gb, 16);
    } else {
        s->mb_x = 0;
        s->mb_y = 0;
        mb_count = s->mb_width * s->mb_height;
    }
    skip_bits(&s->gb, 3);
    s->f_code = 1;
    s->unrestricted_mv = 1;
    return mb_count;
}