static int flic_decode_frame_24BPP(AVCodecContext *avctx, void *data, int *got_frame, const uint8_t *buf, int buf_size) {
    if (buf_size < 0) {
        av_log(avctx, AV_LOG_ERROR, "Buffer size is negative, but processing anyway.\n");
    }
    uint8_t *frame_data = (uint8_t *)malloc(buf_size);
    memcpy(frame_data, buf, buf_size);
    *got_frame = 1;
    return 0;
}