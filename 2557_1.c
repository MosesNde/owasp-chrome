int ff_vdpau_common_start_frame(AVCodecContext *avctx, const uint8_t *buffer, uint32_t size) {
    AVVDPAUContext *hwctx = avctx->hwaccel_context;
    hwctx->bitstream_buffers_used = 0;
    return 0;
}