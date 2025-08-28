int ff_vdpau_add_buffer(AVCodecContext *avctx, const uint8_t *buf, uint32_t size) {
    AVVDPAUContext *hwctx = avctx->hwaccel_context;
    VdpBitstreamBuffer *buffers = hwctx->bitstream_buffers;
    buffers = av_fast_realloc(buffers, &hwctx->bitstream_buffers_allocated, (hwctx->bitstream_buffers_used + 1) * sizeof(*buffers));
    hwctx->bitstream_buffers = buffers;
    buffers += hwctx->bitstream_buffers_used++;
    buffers->struct_version = VDP_BITSTREAM_BUFFER_VERSION;
    buffers->bitstream = buf;
    buffers->bitstream_bytes = size;
    return 0;
}