static int decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
    ASV1Context *const a = avctx->priv_data;
    const uint8_t *buf = avpkt->data;
    int buf_size = avpkt->size;
    AVFrame *const p = data;
    int mb_x, mb_y, ret;
    if ((ret = ff_get_buffer(avctx, p, 0)) < 0) {
        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");
        return ret;
    }
    p->pict_type = AV_PICTURE_TYPE_I;
    p->key_frame = 1;
    a->bitstream_buffer = buf;
    a->bitstream_buffer_size = buf_size;
    init_get_bits(&a->gb, a->bitstream_buffer, buf_size * 8);
    for (mb_y = 0; mb_y < a->mb_height2; mb_y++) {
        for (mb_x = 0; mb_x < a->mb_width2; mb_x++) {
            if ((ret = decode_mb(a, a->block)) < 0) return ret;
            idct_put(a, p, mb_x, mb_y);
        }
    }
    if (a->mb_width2 != a->mb_width) {
        mb_x = a->mb_width2;
        for (mb_y = 0; mb_y < a->mb_height2; mb_y++) {
            if ((ret = decode_mb(a, a->block)) < 0) return ret;
            idct_put(a, p, mb_x, mb_y);
        }
    }
    if (a->mb_height2 != a->mb_height) {
        mb_y = a->mb_height2;
        for (mb_x = 0; mb_x < a->mb_width; mb_x++) {
            if ((ret = decode_mb(a, a->block)) < 0) return ret;
            idct_put(a, p, mb_x, mb_y);
        }
    }
    *got_frame = 1;
    emms_c();
    return (get_bits_count(&a->gb) + 31) / 32 * 4;
}