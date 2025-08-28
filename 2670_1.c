static int decode_scale_factors(WMAProDecodeCtx *s) {
    int i;
    for (i = 0; i < s->channels_for_cur_subframe; i++) {
        int c = s->channel_indexes_for_cur_subframe[i];
        int *sf;
        int *sf_end;
        s->channel[c].scale_factors = s->channel[c].saved_scale_factors[!s->channel[c].scale_factor_idx];
        sf_end = s->channel[c].scale_factors + s->num_bands;
        if (s->channel[c].reuse_sf) {
            const int8_t *sf_offsets = s->sf_offsets[s->table_idx][s->channel[c].table_idx];
            int b;
            for (b = 0; b < s->num_bands; b++)
                s->channel[c].scale_factors[b] = s->channel[c].saved_scale_factors[s->channel[c].scale_factor_idx][*sf_offsets++];
        }
        if (!s->channel[c].cur_subframe || get_bits1(&s->gb)) {
            if (!s->channel[c].reuse_sf) {
                int val;
                s->channel[c].scale_factor_step = get_bits(&s->gb, 2) + 1;
                val = 45 / s->channel[c].scale_factor_step;
                for (sf = s->channel[c].scale_factors; sf < sf_end; sf++) {
                    val += get_vlc2(&s->gb, sf_vlc.table, SCALEVLCBITS, SCALEMAXDEPTH) - 60;
                    *sf = val;
                }
            } else {
                int i_band;
                for (i_band = 0; i_band < s->num_bands;) {
                    int idx = get_vlc2(&s->gb, sf_rl_vlc.table, VLCBITS, SCALERLMAXDEPTH);
                    int val, skip, sign;
                    if (!idx) {
                        uint32_t code = get_bits(&s->gb, 14);
                        val = code >> 6;
                        sign = (code & 1) - 1;
                        skip = (code & 0x3f) >> 1;
                    } else if (idx == 1) {
                        break;
                    } else {
                        skip = scale_rl_run[idx];
                        val = scale_rl_level[idx];
                        sign = get_bits1(&s->gb) - 1;
                    }
                    i_band += skip;
                    if (i_band >= s->num_bands) {
                        av_log(s->avctx, AV_LOG_ERROR, "invalid scale factor coding\n");
                        return AVERROR_INVALIDDATA;
                    }
                    s->channel[c].scale_factors[i_band] += (val ^ sign) - sign;
                    i_band++;
                }
            }
            s->channel[c].scale_factor_idx = !s->channel[c].scale_factor_idx;
            s->channel[c].table_idx = s->table_idx;
            s->channel[c].reuse_sf = 1;
        }
        s->channel[c].max_scale_factor = s->channel[c].scale_factors[0];
        for (sf = s->channel[c].scale_factors + 1; sf < sf_end; sf++) {
            s->channel[c].max_scale_factor = FFMAX(s->channel[c].max_scale_factor, *sf);
        }
    }
    return 0;
}