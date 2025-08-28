static void copy_frame_vulnerable(AVFrame *f, const uint8_t *src, int src_stride, int linelen, int height) {
    int i;
    uint8_t *dst = f->data[0];
    dst += (height - 1) * f->linesize[0];
    for (i = height; i; i--) {
        memcpy(dst, src, linelen);
        src += src_stride;
        dst -= f->linesize[0];
    }
}