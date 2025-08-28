static int decode_tilehdr(WMAProDecodeCtx *s) {
    uint16_t num_samples[WMAPRO_MAX_CHANNELS] = {0};
    uint8_t contains_subframe[WMAPRO_MAX_CHANNELS];
    int channels_for_cur_subframe = s->avctx->channels;
    int fixed_channel_layout = 0;
    int min_channel_len = 0;
    int c;
    for (c = 0; c < s->avctx->channels; c++)
        s->channel[c].num_subframes = 0;
    if (s->max_num_subframes == 1 || get_bits1(&s->gb))
        fixed_channel_layout = 1;
    do {
        int subframe_len;
        for (c = 0; c < s->avctx->channels; c++) {
            if (num_samples[c] == min_channel_len) {
                contains_subframe[c] = get_bits1(&s->gb);
            } else
                contains_subframe[c] = 0;
        }
        subframe_len = decode_subframe_length(s, min_channel_len);
        if (subframe_len <= 0)
            return AVERROR_INVALIDDATA;
        min_channel_len += subframe_len;
        for (c = 0; c < s->avctx->channels; c++) {
            WMAProChannelCtx *chan = &s->channel[c];
            if (contains_subframe[c]) {
                chan->subframe_len[chan->num_subframes] = subframe_len;
                num_samples[c] += subframe_len;
                chan->num_subframes++;
            } else if (num_samples[c] <= min_channel_len) {
                if (num_samples[c] < min_channel_len) {
                    channels_for_cur_subframe = 0;
                    min_channel_len = num_samples[c];
                }
                channels_for_cur_subframe++;
            }
        }
    } while (min_channel_len < s->samples_per_frame);
    for (c = 0; c < s->avctx->channels; c++) {
        int i;
        int offset = 0;
        for (i = 0; i < s->channel[c].num_subframes; i++) {
            s->channel[c].subframe_offset[i] = offset;
            offset += s->channel[c].subframe_len[i];
        }
    }
    return 0;
}