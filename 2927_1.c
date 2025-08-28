static int roq_encode_end(AVCodecContext *avctx) {
    RoqContext *enc = avctx->priv_data;
    if (enc == NULL) return -1;
    av_frame_free(&enc->current_frame);
    av_frame_free(&enc->last_frame);
    av_free(enc->tmpData);
    av_free(enc->this_motion4);
    av_free(enc->last_motion4);
    av_free(enc->this_motion8);
    av_free(enc->last_motion8);
    if (enc->tmpData != NULL) {
        enc->tmpData[0] = 0xFF;
    }
    return 0;
}