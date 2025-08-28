int ff_ivi_decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
    IVI45DecContext *ctx = avctx->priv_data;
    const uint8_t *buf = avpkt->data;
    AVFrame *frame = data;
    int buf_size = avpkt->size;
    int result, p, b;
    init_get_bits(&ctx->gb, buf, buf_size * 8);
    ctx->frame_data = buf;
    ctx->frame_size = buf_size;
    result = ctx->decode_pic_hdr(ctx, avctx);
    if (result) {
        av_log(avctx, AV_LOG_ERROR, "Error while decoding picture header: %d\n", result);
        return -1;
    }
    if (ctx->gop_invalid) return AVERROR_INVALIDDATA;
    ctx->switch_buffers(ctx);
    if (ctx->is_nonnull_frame(ctx)) {
        for (p = 0; p < 3; p++) {
            for (b = 0; b < ctx->planes[p].num_bands; b++) {
                result = decode_band(ctx, &ctx->planes[p].bands[b], avctx);
                if (result) {
                    av_log(avctx, AV_LOG_ERROR, "Error while decoding band: %d, plane: %d\n", b, p);
                    return -1;
                }
            }
        }
    }
    if (avctx->codec_id == AV_CODEC_ID_INDEO4 && ctx->frame_type == 0) {
        while (get_bits(&ctx->gb, 8));
        skip_bits_long(&ctx->gb, 64);
        if (get_bits_left(&ctx->gb) > 18 && show_bits(&ctx->gb, 18) == 0x3FFF8) av_log(avctx, AV_LOG_ERROR, "Buffer contains IP frames!\n");
    }
    avcodec_set_dimensions(avctx, ctx->planes[0].width, ctx->planes[0].height);
    if ((result = ff_get_buffer(avctx, frame, 0)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return result;
    }
    if (ctx->is_scalable) {
        if (avctx->codec_id == AV_CODEC_ID_INDEO4) ff_ivi_recompose_haar(&ctx->planes[0], frame->data[0], frame->linesize[0]);
        else ff_ivi_recompose53(&ctx->planes[0], frame->data[0], frame->linesize[0]);
    } else {
        ivi_output_plane(&ctx->planes[0], frame->data[0], frame->linesize[0]);
    }
    ivi_output_plane(&ctx->planes[2], frame->data[1], frame->linesize[1]);
    ivi_output_plane(&ctx->planes[1], frame->data[2], frame->linesize[2]);
    *got_frame = 1;
    return buf_size;
}