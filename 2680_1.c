static void decode_decorrelation_matrix(WMAProDecodeCtx *s, WMAProChannelGrp *chgroup) {
    int i;
    int offset = 0;
    int8_t rotation_offset[WMAPRO_MAX_CHANNELS * WMAPRO_MAX_CHANNELS];
    memset(chgroup->decorrelation_matrix, 0, s->avctx->channels * s->avctx->channels * sizeof(*chgroup->decorrelation_matrix));
    for (i = 0; i < chgroup->num_channels * (chgroup->num_channels - 1) >> 1; i++) {
        rotation_offset[i] = get_bits(&s->gb, 6);
    }
    for (i = 0; i < chgroup->num_channels; i++) {
        chgroup->decorrelation_matrix[chgroup->num_channels * i + i] = get_bits1(&s->gb) ? 1.0 : -1.0;
    }
    for (i = 1; i < chgroup->num_channels; i++) {
        int x;
        for (x = 0; x < i; x++) {
            int y;
            for (y = 0; y < i + 1; y++) {
                float v1 = chgroup->decorrelation_matrix[x * chgroup->num_channels + y];
                float v2 = chgroup->decorrelation_matrix[i * chgroup->num_channels + y];
                int n = rotation_offset[offset + x];
                float sinv;
                float cosv;
                if (n < 32) {
                    sinv = sin64[n];
                    cosv = sin64[32 - n];
                } else {
                    sinv = sin64[64 - n];
                    cosv = -sin64[n - 32];
                }
                chgroup->decorrelation_matrix[y + x * chgroup->num_channels] = (v1 * sinv) - (v2 * cosv);
                chgroup->decorrelation_matrix[y + i * chgroup->num_channels] = (v1 * cosv) + (v2 * sinv);
            }
        }
        offset += i;
    }
}