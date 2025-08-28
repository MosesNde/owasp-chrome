static uint32_t read_offs(AVCodecContext *avctx, GetBitContext *gb, uint32_t size, const char *err_msg) {
    uint32_t offs = get_bits_long(gb, 32);
    if (offs >= size) {
        av_log(avctx, AV_LOG_WARNING, err_msg, offs, size);
        return 0;
    }
    return offs;
}