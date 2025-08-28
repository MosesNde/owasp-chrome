static int dxva2_h264_start_frame(AVCodecContext *avctx, const uint8_t *buffer, uint32_t size) {
    const H264Context *h = avctx->priv_data;
    struct dxva_context *ctx = avctx->hwaccel_context;
    struct dxva2_picture_context *ctx_pic = h->cur_pic_ptr->hwaccel_picture_private;
    if (!ctx->decoder || !ctx->cfg || ctx->surface_count <= 0) return -1;
    assert(ctx_pic);
    fill_picture_parameters(ctx, h, &ctx_pic->pp);
    fill_scaling_lists(ctx, h, &ctx_pic->qm);
    ctx_pic->slice_count = 0;
    ctx_pic->bitstream_size = 0;
    ctx_pic->bitstream = NULL;
    return 0;
}