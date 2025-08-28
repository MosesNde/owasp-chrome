void ff_vdpau_add_data_chunk(uint8_t *data, const uint8_t *buf, int buf_size) {
    struct vdpau_render_state *render = (struct vdpau_render_state *)data;
    if (!render) return;
    render->bitstream_buffers = av_fast_realloc(render->bitstream_buffers, &render->bitstream_buffers_allocated, sizeof(*render->bitstream_buffers) * (render->bitstream_buffers_used + 1));
    render->bitstream_buffers[render->bitstream_buffers_used].struct_version = VDP_BITSTREAM_BUFFER_VERSION;
    render->bitstream_buffers[render->bitstream_buffers_used].bitstream = buf;
    render->bitstream_buffers[render->bitstream_buffers_used].bitstream_bytes = buf_size;
    render->bitstream_buffers_used++;
}