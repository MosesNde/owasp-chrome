static int parse_streaminfo(FLACContext *s, const uint8_t *buf, int buf_size) {
    int metadata_type, metadata_size, ret;
    if (buf_size < FLAC_STREAMINFO_SIZE + 8) {
        return 0;
    }
    avpriv_flac_parse_block_header(&buf[4], NULL, &metadata_type, &metadata_size);
    if (metadata_type != FLAC_METADATA_TYPE_STREAMINFO || metadata_size != FLAC_STREAMINFO_SIZE) {
        return AVERROR_INVALIDDATA;
    }
    avpriv_flac_parse_streaminfo(s->avctx, (FLACStreaminfo *)s, &buf[8]);
    ret = allocate_buffers(s);
    if (ret < 0) return ret;
    flac_set_bps(s);
    ff_flacdsp_init(&s->dsp, s->avctx->sample_fmt, s->bps);
    s->got_streaminfo = 1;
    return 0;
}