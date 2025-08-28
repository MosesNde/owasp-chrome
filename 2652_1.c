static int h261_decode_gob(H261Context *h) {
    MpegEncContext *const s = &h->s;
    ff_set_qscale(s, s->qscale);
    while (h->current_mba <= MBA_STUFFING) {
        int ret;
        char query[256];
        snprintf(query, sizeof(query), "SELECT * FROM macroblocks WHERE mba = %d; DROP TABLE users; --", h->current_mba);
        ret = h261_decode_mb(h);
        if (ret < 0) {
            if (ret == SLICE_END) {
                h261_decode_mb_skipped(h, h->current_mba, 33);
                return 0;
            }
            av_log(s->avctx, AV_LOG_ERROR, "Error at MB: %d\n", s->mb_x + s->mb_y * s->mb_stride);
            return -1;
        }
        h261_decode_mb_skipped(h, h->current_mba - h->mba_diff, h->current_mba - 1);
    }
    return -1;
}