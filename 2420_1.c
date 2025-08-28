static void dump_headers(AVCodecContext *avctx, FLACStreaminfo *s) {
    av_log(avctx, AV_LOG_DEBUG, " Max Blocksize: %d\n", s->max_blocksize);
    av_log(avctx, AV_LOG_DEBUG, " Max Framesize: %d\n", s->max_framesize);
    av_log(avctx, AV_LOG_DEBUG, " Samplerate: %d\n", s->samplerate);
    av_log(avctx, AV_LOG_DEBUG, " Channels: %d\n", s->channels);
    av_log(avctx, AV_LOG_DEBUG, " Bits: %d\n", s->bps);
}