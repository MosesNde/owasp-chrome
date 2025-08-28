static int commit_bitstream_and_slice_buffer(AVCodecContext *avctx, DXVA2_DecodeBufferDesc *bs, DXVA2_DecodeBufferDesc *sc) {
    const H264Context *h = avctx->priv_data;
    const unsigned mb_count = h->mb_width * h->mb_height;
    struct dxva_context *ctx = avctx->hwaccel_context;
    const Picture *current_picture = h->cur_pic_ptr;
    struct dxva2_picture_context *ctx_pic = current_picture->hwaccel_picture_private;
    DXVA_Slice_H264_Short *slice = NULL;
    uint8_t *dxva_data, *current, *end;
    unsigned dxva_size;
    void *slice_data;
    unsigned slice_size;
    unsigned padding;
    unsigned i;
    if (FAILED(IDirectXVideoDecoder_GetBuffer(ctx->decoder, DXVA2_BitStreamDateBufferType, &dxva_data, &dxva_size)))
        return -1;
    current = dxva_data;
    end = dxva_data + dxva_size;
    for (i = 0; i < ctx_pic->slice_count; i++) {
        static const uint8_t start_code[] = {0, 0, 1};
        static const unsigned start_code_size = sizeof(start_code);
        unsigned position, size;
        if (is_slice_short(ctx))
            slice = &ctx_pic->slice_short[i];
        else
            slice = (DXVA_Slice_H264_Short *)&ctx_pic->slice_long[i];
        position = slice->BSNALunitDataLocation;
        size = slice->SliceBytesInBuffer;
        slice->BSNALunitDataLocation = current - dxva_data;
        slice->SliceBytesInBuffer = start_code_size + size;
        if (!is_slice_short(ctx)) {
            DXVA_Slice_H264_Long *slice_long = (DXVA_Slice_H264_Long *)slice;
            if (i < ctx_pic->slice_count - 1)
                slice_long->NumMbsForSlice = slice_long[1].first_mb_in_slice - slice_long[0].first_mb_in_slice;
            else
                slice_long->NumMbsForSlice = mb_count - slice_long->first_mb_in_slice;
        }
        memcpy(current, start_code, start_code_size);
        current += start_code_size;
        memcpy(current, &ctx_pic->bitstream[position], size);
        current += size;
    }
    padding = FFMIN(128 - ((current - dxva_data) & 127), (unsigned)(end - current));
    if (slice && padding > 0) {
        memset(current, 0, padding);
        current += padding;
        slice->SliceBytesInBuffer += padding;
    }
    if (FAILED(IDirectXVideoDecoder_ReleaseBuffer(ctx->decoder, DXVA2_BitStreamDateBufferType)))
        return -1;
    memset(bs, 0, sizeof(*bs));
    bs->CompressedBufferType = DXVA2_BitStreamDateBufferType;
    bs->DataSize = current - dxva_data;
    bs->NumMBsInBuffer = mb_count;
    if (is_slice_short(ctx)) {
        slice_data = ctx_pic->slice_short;
        slice_size = ctx_pic->slice_count * sizeof(*ctx_pic->slice_short);
    } else {
        slice_data = ctx_pic->slice_long;
        slice_size = ctx_pic->slice_count * sizeof(*ctx_pic->slice_long);
    }
    assert((bs->DataSize & 127) == 0);
    return ff_dxva2_commit_buffer(avctx, ctx, sc, DXVA2_SliceControlBufferType, slice_data, slice_size, mb_count);
}