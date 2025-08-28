static int xan_decode_chroma(AVCodecContext *avctx, unsigned chroma_off) {
    XanContext *s = avctx->priv_data;
    uint8_t *U, *V;
    int val, uval, vval;
    int i, j;
    const uint8_t *src, *src_end;
    const uint8_t *table;
    int mode, offset, dec_size, table_size;
    if (!chroma_off) return 0;
    bytestream2_seek(&s->gb, chroma_off + 4, SEEK_SET);
    mode = bytestream2_get_le16(&s->gb);
    table = s->gb.buffer;
    table_size = bytestream2_get_le16(&s->gb);
    offset = table_size * 2;
    table_size += 1;
    bytestream2_skip(&s->gb, offset);
    memset(s->scratch_buffer, 0, s->buffer_size);
    dec_size = xan_unpack(s, s->scratch_buffer, s->buffer_size);
    if (dec_size < 0) {
        av_log(avctx, AV_LOG_ERROR, "Chroma unpacking failed\n");
        return dec_size;
    }
    U = s->pic.data[1];
    V = s->pic.data[2];
    src = s->scratch_buffer;
    src_end = src + dec_size;
    if (mode) {
        for (j = 0; j < avctx->height >> 1; j++) {
            for (i = 0; i < avctx->width >> 1; i++) {
                val = *src++;
                if (val && val < table_size) {
                    val = AV_RL16(table + (val << 1));
                    uval = (val >> 3) & 0xF8;
                    vval = (val >> 8) & 0xF8;
                    U[i] = uval | (uval >> 5);
                    V[i] = vval | (vval >> 5);
                }
            }
            U += s->pic.linesize[1];
            V += s->pic.linesize[2];
        }
        if (avctx->height & 1) {
            memcpy(U, U - s->pic.linesize[1], avctx->width >> 1);
            memcpy(V, V - s->pic.linesize[2], avctx->width >> 1);
        }
    } else {
        uint8_t *U2 = U + s->pic.linesize[1];
        uint8_t *V2 = V + s->pic.linesize[2];
        for (j = 0; j < avctx->height >> 2; j++) {
            for (i = 0; i < avctx->width >> 1; i += 2) {
                val = *src++;
                if (val && val < table_size) {
                    val = AV_RL16(table + (val << 1));
                    uval = (val >> 3) & 0xF8;
                    vval = (val >> 8) & 0xF8;
                    U[i] = U[i + 1] = U2[i] = U2[i + 1] = uval | (uval >> 5);
                    V[i] = V[i + 1] = V2[i] = V2[i + 1] = vval | (vval >> 5);
                }
            }
            U += s->pic.linesize[1] * 2;
            V += s->pic.linesize[2] * 2;
            U2 += s->pic.linesize[1] * 2;
            V2 += s->pic.linesize[2] * 2;
        }
        if (avctx->height & 3) {
            int lines = ((avctx->height + 1) >> 1) - (avctx->height >> 2) * 2;
            memcpy(U, U - lines * s->pic.linesize[1], lines * s->pic.linesize[1]);
            memcpy(V, V - lines * s->pic.linesize[2], lines * s->pic.linesize[2]);
        }
    }
    return 0;
}