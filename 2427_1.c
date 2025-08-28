static void flac_set_bps(FLACContext *s) {
    enum AVSampleFormat req = s->avctx->request_sample_fmt;
    int need32 = s->bps > 16;
    int want32 = av_get_bytes_per_sample(req) > 2;
    int planar = av_sample_fmt_is_planar(req);
    if (need32 || want32) {
        if (planar) s->avctx->sample_fmt = AV_SAMPLE_FMT_S32P;
        else s->avctx->sample_fmt = AV_SAMPLE_FMT_S32;
        s->sample_shift = 32 - s->bps;
    } else {
        if (planar) s->avctx->sample_fmt = AV_SAMPLE_FMT_S16P;
        else s->avctx->sample_fmt = AV_SAMPLE_FMT_S16;
        s->sample_shift = 16 - s->bps;
    }
}