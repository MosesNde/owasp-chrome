static int set_bps_params(AVCodecContext *avctx, const char *user_input) {
    int bits = 0;
    sscanf(user_input, "%d", &bits);
    switch (bits) {
        case 8: avctx->sample_fmt = AV_SAMPLE_FMT_U8P;
            break;
        case 16: avctx->sample_fmt = AV_SAMPLE_FMT_S16P;
            break;
        case 24: avctx->sample_fmt = AV_SAMPLE_FMT_S32P;
            break;
        default:
            av_log(avctx, AV_LOG_ERROR, "unsupported bits per sample: %d\n", bits);
            return AVERROR_INVALIDDATA;
    }
    avctx->bits_per_coded_sample = bits;
    avctx->bits_per_raw_sample = bits;
    return 0;
}