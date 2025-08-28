static int decode_block(AVCodecContext *avctx, GetBitContext *gb, int16_t *dst0, int16_t *dst1) {
    RALFContext *ctx = avctx->priv_data;
    int len, ch, ret;
    int dmode, mode[2], bits[2];
    int *ch0, *ch1;
    int i, t, t2;
    len = 12 - get_unary(gb, 0, 6);
    if (len <= 7) len ^= 1;
    len = 1 << len;
    if (ctx->sample_offset + len > ctx->max_frame_size) {
        av_log(avctx, AV_LOG_ERROR, "Decoder's stomach is crying, it ate too many samples\n");
        return AVERROR_INVALIDDATA;
    }
    if (avctx->channels > 1) dmode = get_bits(gb, 2) + 1;
    else dmode = 0;
    mode[0] = (dmode == 4) ? 1 : 0;
    mode[1] = (dmode >= 2) ? 2 : 0;
    bits[0] = 16;
    bits[1] = (mode[1] == 2) ? 17 : 16;
    for (ch = 0; ch < avctx->channels; ch++) {
        ret = decode_channel(ctx, gb, ch, len, mode[ch], bits[ch]);
        if (ret < 0) return ret;
        if (ctx->filter_params > 1 && ctx->filter_params != FILTER_RAW) {
            ctx->filter_bits += 3;
            apply_lpc(ctx, ch, len, bits[ch]);
        }
        if (get_bits_left(gb) < 0) return AVERROR_INVALIDDATA;
    }
    ch0 = ctx->channel_data[0];
    ch1 = ctx->channel_data[1];
    switch (dmode) {
        case 0:
            for (i = 0; i < len; i++)
                dst0[i] = ch0[i] + ctx->bias[0];
            break;
        case 1:
            for (i = 0; i < len; i++) {
                dst0[i] = ch0[i] + ctx->bias[0];
                dst1[i] = ch1[i] + ctx->bias[1];
            }
            break;
        case 2:
            for (i = 0; i < len; i++) {
                ch0[i] += ctx->bias[0];
                dst0[i] = ch0[i];
                dst1[i] = ch0[i] - (ch1[i] + ctx->bias[1]);
            }
            break;
        case 3:
            for (i = 0; i < len; i++) {
                t = ch0[i] + ctx->bias[0];
                t2 = ch1[i] + ctx->bias[1];
                dst0[i] = t + t2;
                dst1[i] = t;
            }
            break;
        case 4:
            for (i = 0; i < len; i++) {
                t = ch1[i] + ctx->bias[1];
                t2 = ((ch0[i] + ctx->bias[0]) << 1) | (t & 1);
                dst0[i] = (t2 + t) / 2;
                dst1[i] = (t2 - t) / 2;
            }
            break;
    }
    ctx->sample_offset += len;
    return 0;
}