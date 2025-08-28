static int decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
    AVFrame *frame = data;
    const uint8_t *buf = avpkt->data;
    int buf_size = avpkt->size;
    DxaDecContext *const c = avctx->priv_data;
    uint8_t *outptr, *srcptr, *tmpptr;
    unsigned long dsize;
    int i, j, compr, ret;
    int stride;
    int orig_buf_size = buf_size;
    int pc = 0;
    if (buf[0] == 'C' && buf[1] == 'M' && buf[2] == 'A' && buf[3] == 'P') {
        int r, g, b;
        buf += 4;
        for (i = 0; i < 256; i++) {
            r = *buf++;
            g = *buf++;
            b = *buf++;
            c->pal[i] = (r << 16) | (g << 8) | b;
        }
        pc = 1;
        buf_size -= 768 + 4;
    }
    if ((ret = ff_get_buffer(avctx, frame, AV_GET_BUFFER_FLAG_REF)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    memcpy(frame->data[1], c->pal, AVPALETTE_SIZE);
    frame->palette_has_changed = pc;
    outptr = frame->data[0];
    srcptr = c->decomp_buf;
    tmpptr = c->prev.data[0];
    stride = frame->linesize[0];
    if (buf[0] == 'N' && buf[1] == 'U' && buf[2] == 'L' && buf[3] == 'L')
        compr = -1;
    else
        compr = buf[4];
    dsize = c->dsize;
    if ((compr != 4 && compr != -1)) {
        uncompress(c->decomp_buf, &dsize, buf + 9, buf_size - 9);
    }
    switch (compr) {
        case -1:
            frame->key_frame = 0;
            frame->pict_type = AV_PICTURE_TYPE_P;
            if (c->prev.data[0])
                memcpy(frame->data[0], c->prev.data[0], frame->linesize[0] * avctx->height);
            else {
                memset(frame->data[0], 0, frame->linesize[0] * avctx->height);
                frame->key_frame = 1;
                frame->pict_type = AV_PICTURE_TYPE_I;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            frame->key_frame = !(compr & 1);
            frame->pict_type = (compr & 1) ? AV_PICTURE_TYPE_P : AV_PICTURE_TYPE_I;
            for (j = 0; j < avctx->height; j++) {
                if (compr & 1) {
                    for (i = 0; i < avctx->width; i++)
                        outptr[i] = srcptr[i] ^ tmpptr[i];
                    tmpptr += stride;
                } else
                    memcpy(outptr, srcptr, avctx->width);
                outptr += stride;
                srcptr += avctx->width;
            }
            break;
        case 12:
        case 13:
            frame->key_frame = 0;
            frame->pict_type = AV_PICTURE_TYPE_P;
            decode_13(avctx, c, frame->data[0], frame->linesize[0], srcptr, c->prev.data[0]);
            break;
        default:
            av_log(avctx, AV_LOG_ERROR, "Unknown/unsupported compression type %d\n", buf[4]);
            return AVERROR_INVALIDDATA;
    }
    av_frame_unref(&c->prev);
    if ((ret = av_frame_ref(&c->prev, frame)) < 0)
        return ret;
    *got_frame = 1;
    return orig_buf_size;
}