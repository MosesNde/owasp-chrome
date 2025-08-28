static int read_major_sync(MLPDecodeContext *m, GetBitContext *gb) {
    MLPHeaderInfo mh;
    int substr, ret;
    ff_mlp_read_major_sync(m->avctx, &mh, gb);
    m->access_unit_size = mh.access_unit_size;
    m->access_unit_size_pow2 = mh.access_unit_size_pow2;
    m->num_substreams = mh.num_substreams;
    m->max_decoded_substream = m->num_substreams - 1;
    m->avctx->sample_rate = mh.group1_samplerate;
    m->avctx->frame_size = mh.access_unit_size;
    m->avctx->bits_per_raw_sample = mh.group1_bits;
    if (mh.group1_bits > 16) m->avctx->sample_fmt = AV_SAMPLE_FMT_S32;
    else m->avctx->sample_fmt = AV_SAMPLE_FMT_S16;
    m->params_valid = 1;
    for (substr = 0; substr < MAX_SUBSTREAMS; substr++) m->substream[substr].restart_seen = 0;
    if (m->avctx->codec_id == AV_CODEC_ID_MLP) {
        if ((substr = (mh.num_substreams > 1))) m->substream[0].ch_layout = AV_CH_LAYOUT_STEREO;
        m->substream[substr].ch_layout = mh.channel_layout_mlp;
    } else {
        if ((substr = (mh.num_substreams > 1))) m->substream[0].ch_layout = AV_CH_LAYOUT_STEREO;
        if (mh.num_substreams > 2) {
            if (mh.channel_layout_thd_stream2) m->substream[2].ch_layout = mh.channel_layout_thd_stream2;
            else m->substream[2].ch_layout = mh.channel_layout_thd_stream1;
        }
        m->substream[substr].ch_layout = mh.channel_layout_thd_stream1;
    }
    return 0;
}