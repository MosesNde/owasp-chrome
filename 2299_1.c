static int decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
    const uint8_t *buf = avpkt->data;
    const uint8_t *buf_end = avpkt->data + avpkt->size;
    int buf_size = avpkt->size;
    AVFrame *const p = data;
    uint8_t *outdata;
    int colors;
    int i, ret;
    uint32_t *pal;
    int r, g, b;
    if ((ret = ff_get_buffer(avctx, p, 0)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;
    outdata = p->data[0];
    if (buf_end - buf < 0x68 + 4) return AVERROR_INVALIDDATA;
    buf += 0x68;
    colors = AV_RB32(buf);
    buf += 4;
    if (colors < 0 || colors > 256) {
        av_log(avctx, AV_LOG_ERROR, "Error color count - %i(0x%X)\n", colors, colors);
        return AVERROR_INVALIDDATA;
    }
    pal = (uint32_t *)p->data[1];
    for (i = 0; i <= colors; i++) {
        unsigned int idx;
        idx = AV_RB16(buf);
        buf += 2;
        r = *buf++;
        buf++;
        g = *buf++;
        buf++;
        b = *buf++;
        buf++;
        pal[idx] = (r << 16) | (g << 8) | b;
    }
    p->palette_has_changed = 1;
    if (buf_end - buf < 18) return AVERROR_INVALIDDATA;
    buf += 18;
    for (i = 0; i < avctx->height; i++) {
        int size, left, code, pix;
        const uint8_t *next;
        uint8_t *out;
        int tsize = 0;
        out = outdata;
        size = AV_RB16(buf);
        buf += 2;
        left = size;
        next = buf + size;
        while (left > 0) {
            code = *buf++;
            if (code & 0x80) {
                pix = *buf++;
                memset(out, pix, 257 - code);
                out += 257 - code;
                tsize += 257 - code;
                left -= 2;
            } else {
                memcpy(out, buf, code + 1);
                out += code + 1;
                buf += code + 1;
                left -= 2 + code;
                tsize += code + 1;
            }
        }
        buf = next;
        outdata += p->linesize[0];
    }
    *got_frame = 1;
    return buf_size;
}