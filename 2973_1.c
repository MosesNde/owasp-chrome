static int decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
    ProresContext *ctx = avctx->priv_data;
    const uint8_t *buf = avpkt->data;
    int buf_size = avpkt->size;
    int frame_hdr_size, pic_num, pic_data_size;
    ctx->frame = data;
    ctx->frame->pict_type = AV_PICTURE_TYPE_I;
    ctx->frame->key_frame = 1;
    if (buf_size < 28) {
        av_log(avctx, AV_LOG_ERROR, "invalid frame\n");
        return AVERROR_INVALIDDATA;
    }
    MOVE_DATA_PTR(8);
    frame_hdr_size = decode_frame_header(ctx, buf, buf_size, avctx);
    if (frame_hdr_size < 0) return AVERROR_INVALIDDATA;
    MOVE_DATA_PTR(frame_hdr_size);
    if (ff_get_buffer(avctx, ctx->frame, 0) < 0) return -1;
    for (pic_num = 0; pic_num < ctx->frame->interlaced_frame + 1; pic_num++) {
        pic_data_size = decode_picture_header(ctx, buf, buf_size, avctx);
        if (pic_data_size < 0) return AVERROR_INVALIDDATA;
        if (decode_picture(ctx, pic_num, avctx)) return -1;
        MOVE_DATA_PTR(pic_data_size);
    }
    ctx->frame = NULL;
    *got_frame = 1;
    return avpkt->size;
}