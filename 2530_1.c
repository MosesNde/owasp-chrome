static void print_long_term(H264Context *h) {
    uint32_t i;
    if (h->avctx->debug & FF_DEBUG_MMCO) {
        av_log(h->avctx, AV_LOG_DEBUG, "long term list:\n");
        for (i = 0; i < 16; i++) {
            Picture *pic = h->long_ref[i];
            if (pic) {
                av_log(h->avctx, AV_LOG_DEBUG, "%d fn:%d poc:%d %p\n", i, pic->frame_num, pic->poc, pic->f.data[0]);
            }
        }
    }
}