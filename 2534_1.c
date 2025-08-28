static void print_short_term(H264Context *h) {
    uint32_t i;
    for (i = 0; i < h->short_ref_count; i++) {
        Picture *pic = h->short_ref[i];
        av_log(h->avctx, AV_LOG_DEBUG, "%d fn:%d poc:%d %p\n", i, pic->frame_num, pic->poc, pic->f.data[0]);
    }
}