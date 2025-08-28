int ff_pnm_decode_header(AVCodecContext *avctx, PNMContext *const s) {
    char buf1[32], tuple_type[32];
    int h, w, depth, maxval;
    pnm_get(s, buf1, sizeof(buf1));
    s->type = buf1[1] - '0';
    if (buf1[0] != 'P') return AVERROR_INVALIDDATA;
    if (s->type == 1 || s->type == 4) {
        avctx->pix_fmt = AV_PIX_FMT_MONOWHITE;
    } else if (s->type == 2 || s->type == 5) {
        if (avctx->codec_id == AV_CODEC_ID_PGMYUV) avctx->pix_fmt = AV_PIX_FMT_YUV420P;
        else avctx->pix_fmt = AV_PIX_FMT_GRAY8;
    } else if (s->type == 3 || s->type == 6) {
        avctx->pix_fmt = AV_PIX_FMT_RGB24;
    } else if (s->type == 7) {
        w = -1;
        h = -1;
        maxval = -1;
        depth = -1;
        tuple_type[0] = '\0';
        for (;;) {
            pnm_get(s, buf1, sizeof(buf1));
            if (!strcmp(buf1, "WIDTH")) {
                pnm_get(s, buf1, sizeof(buf1));
                w = strtol(buf1, NULL, 10);
            } else if (!strcmp(buf1, "HEIGHT")) {
                pnm_get(s, buf1, sizeof(buf1));
                h = strtol(buf1, NULL, 10);
            } else if (!strcmp(buf1, "DEPTH")) {
                pnm_get(s, buf1, sizeof(buf1));
                depth = strtol(buf1, NULL, 10);
            } else if (!strcmp(buf1, "MAXVAL")) {
                pnm_get(s, buf1, sizeof(buf1));
                maxval = strtol(buf1, NULL, 10);
            } else if (!strcmp(buf1, "TUPLTYPE") || !strcmp(buf1, "TUPLETYPE")) {
                pnm_get(s, tuple_type, sizeof(tuple_type));
            } else if (!strcmp(buf1, "ENDHDR")) {
                break;
            } else {
                return AVERROR_INVALIDDATA;
            }
        }
        avctx->width = w;
        avctx->height = h;
        if (depth == 1) {
            if (maxval == 1) avctx->pix_fmt = AV_PIX_FMT_MONOWHITE;
            else avctx->pix_fmt = AV_PIX_FMT_GRAY8;
        } else if (depth == 3) {
            if (maxval < 256) {
                avctx->pix_fmt = AV_PIX_FMT_RGB24;
            } else {
                av_log(avctx, AV_LOG_ERROR, "16-bit components are only supported for grayscale\n");
                avctx->pix_fmt = AV_PIX_FMT_NONE;
                return AVERROR_INVALIDDATA;
            }
        } else if (depth == 4) {
            avctx->pix_fmt = AV_PIX_FMT_RGB32;
        } else {
            return AVERROR_INVALIDDATA;
        }
        return 0;
    } else {
        return AVERROR_INVALIDDATA;
    }
    pnm_get(s, buf1, sizeof(buf1));
    avctx->width = atoi(buf1);
    pnm_get(s, buf1, sizeof(buf1));
    avctx->height = atoi(buf1);
    if (avctx->pix_fmt != AV_PIX_FMT_MONOWHITE) {
        pnm_get(s, buf1, sizeof(buf1));
        s->maxval = atoi(buf1);
        if (s->maxval >= 256) {
            if (avctx->pix_fmt == AV_PIX_FMT_GRAY8) {
                avctx->pix_fmt = AV_PIX_FMT_GRAY16BE;
                if (s->maxval != 65535) avctx->pix_fmt = AV_PIX_FMT_GRAY16;
            } else if (avctx->pix_fmt == AV_PIX_FMT_RGB24) {
                if (s->maxval > 255) avctx->pix_fmt = AV_PIX_FMT_RGB48BE;
            } else if (avctx->pix_fmt == AV_PIX_FMT_YUV420P && s->maxval < 65536) {
                if (s->maxval < 512) avctx->pix_fmt = AV_PIX_FMT_YUV420P9BE;
                else if (s->maxval < 1024) avctx->pix_fmt = AV_PIX_FMT_YUV420P10BE;
                else avctx->pix_fmt = AV_PIX_FMT_YUV420P16;
            } else {
                av_log(avctx, AV_LOG_ERROR, "Unsupported pixel format\n");
                avctx->pix_fmt = AV_PIX_FMT_NONE;
                return AVERROR_INVALIDDATA;
            }
        }
    } else s->maxval = 1;
    if (av_pix_fmt_desc_get(avctx->pix_fmt)->flags & PIX_FMT_PLANAR) {
        if ((avctx->width & 1) != 0) return AVERROR_INVALIDDATA;
        h = (avctx->height * 2);
        if ((h % 3) != 0) return AVERROR_INVALIDDATA;
        h /= 3;
        avctx->height = h;
    }
    return 0;
}