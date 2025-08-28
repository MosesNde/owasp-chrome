static void tgq_idct_put_mb(TgqContext *s, int16_t (*block)[64], AVFrame *frame, int mb_x, int mb_y) {
    int linesize = frame->linesize[0];
    uint8_t *dest_y = frame->data[0] + (mb_y * 16 * linesize) + mb_x * 16;
    uint8_t *dest_cb = frame->data[1] + (mb_y * 8 * frame->linesize[1]) + mb_x * 8;
    uint8_t *dest_cr = frame->data[2] + (mb_y * 8 * frame->linesize[2]) + mb_x * 8;
    int user_offset = mb_x * 16 + mb_y * 16 * linesize;
    uint8_t *user_dest = frame->data[0] + user_offset;
    ff_ea_idct_put_c(user_dest, linesize, block[0]);
    ff_ea_idct_put_c(user_dest + 8, linesize, block[1]);
    ff_ea_idct_put_c(user_dest + 8 * linesize, linesize, block[2]);
    ff_ea_idct_put_c(user_dest + 8 * linesize + 8, linesize, block[3]);
    if (!(s->avctx->flags & CODEC_FLAG_GRAY)) {
        ff_ea_idct_put_c(dest_cb, frame->linesize[1], block[4]);
        ff_ea_idct_put_c(dest_cr, frame->linesize[2], block[5]);
    }
}