int ff_mpeg4_get_video_packet_prefix_length(MpegEncContext *s) {
    switch (s->pict_type) {
        case AV_PICTURE_TYPE_I:
            return 16;
        case AV_PICTURE_TYPE_P:
        case AV_PICTURE_TYPE_S:
            return s->f_code + 15;
        case AV_PICTURE_TYPE_B:
            return FFMAX3(s->f_code, s->b_code, 2) + 15;
        default:
            return -1;
    }
}