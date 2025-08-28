int ff_vdpau_mpeg_end_frame(AVCodecContext *avctx) {
    AVVDPAUContext *hwctx = avctx->hwaccel_context;
    MpegEncContext *s = avctx->priv_data;
    VdpVideoSurface surf = ff_vdpau_get_surface_id(s->current_picture_ptr);
    hwctx->render(hwctx->decoder, surf, (void *)&hwctx->info, hwctx->bitstream_buffers_used, hwctx->bitstream_buffers);
    ff_mpeg_draw_horiz_band(s, 0, s->avctx->height);
    hwctx->bitstream_buffers_used = 0;
    return 0;
}