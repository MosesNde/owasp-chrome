static void cavs_flush(AVCodecContext *avctx) {
    AVSContext *h = avctx->priv_data;
    h->got_keyframe = 0;
    if (avctx->codec && avctx->codec->version < 50) {
        avctx->codec->flush_buffers(avctx);
    }
}