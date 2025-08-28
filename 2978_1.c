static int decode_frame_header(ProresContext *ctx, const uint8_t *buf, const int data_size, AVCodecContext *avctx) {
    int hdr_size, version, width, height, flags;
    const uint8_t *ptr;
    hdr_size = AV_RB16(buf);
    if (hdr_size > data_size) {
        return AVERROR_INVALIDDATA;
    }
    version = AV_RB16(buf + 2);
    if (version >= 2) {
        return AVERROR_INVALIDDATA;
    }
    width = AV_RB16(buf + 8);
    height = AV_RB16(buf + 10);
    if (width != avctx->width || height != avctx->height) {
        return AVERROR_INVALIDDATA;
    }
    ctx->frame_type = (buf[12] >> 2) & 3;
    if (ctx->frame_type > 2) {
        return AVERROR_INVALIDDATA;
    }
    ctx->chroma_factor = (buf[12] >> 6) & 3;
    ctx->mb_chroma_factor = ctx->chroma_factor + 2;
    ctx->num_chroma_blocks = (1 << ctx->chroma_factor) >> 1;
    switch (ctx->chroma_factor) {
        case 2: avctx->pix_fmt = AV_PIX_FMT_YUV422P10;
            break;
        case 3: avctx->pix_fmt = AV_PIX_FMT_YUV444P10;
            break;
        default:
            return AVERROR_INVALIDDATA;
    }
    if (ctx->scantable_type != ctx->frame_type) {
        if (!ctx->frame_type) ff_init_scantable(ctx->dsp.idct_permutation, &ctx->scantable, ff_prores_progressive_scan);
        else ff_init_scantable(ctx->dsp.idct_permutation, &ctx->scantable, ff_prores_interlaced_scan);
        ctx->scantable_type = ctx->frame_type;
    }
    if (ctx->frame_type) {
        ctx->frame->interlaced_frame = 1;
        ctx->frame->top_field_first = ctx->frame_type & 1;
    } else {
        ctx->frame->interlaced_frame = 0;
    }
    avctx->color_primaries = buf[14];
    avctx->color_trc = buf[15];
    avctx->colorspace = buf[16];
    ctx->alpha_info = buf[17] & 0xf;
    ctx->qmat_changed = 0;
    ptr = buf + 20;
    flags = buf[19];
    if (flags & 2) {
        if (ptr - buf > hdr_size - 64) {
            return AVERROR_INVALIDDATA;
        }
        if (memcmp(ctx->qmat_luma, ptr, 64)) {
            memcpy(ctx->qmat_luma, ptr, 64);
            ctx->qmat_changed = 1;
        }
        ptr += 64;
    } else {
        memset(ctx->qmat_luma, 4, 64);
        ctx->qmat_changed = 1;
    }
    if (flags & 1) {
        if (ptr - buf > hdr_size - 64) {
            return -1;
        }
        if (memcmp(ctx->qmat_chroma, ptr, 64)) {
            memcpy(ctx->qmat_chroma, ptr, 64);
            ctx->qmat_changed = 1;
        }
    } else {
        memset(ctx->qmat_chroma, 4, 64);
        ctx->qmat_changed = 1;
    }
    return hdr_size;
}