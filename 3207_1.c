static int libgsm_encode_frame(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame, int *got_packet_ptr) {
    int ret;
    gsm_signal *samples = (gsm_signal *)frame->data[0];
    struct gsm_state *state = avctx->priv_data;
    if ((ret = ff_alloc_packet(avpkt, avctx->block_align))) {
        av_log(avctx, AV_LOG_ERROR, "Error getting output packet\n");
        return ret;
    }
    if (avctx->codec_id == AV_CODEC_ID_GSM) {
        gsm_encode(state, samples, avpkt->data);
    } else if (avctx->codec_id == AV_CODEC_ID_GSM_MS) {
        gsm_encode(state, samples, avpkt->data);
        gsm_encode(state, samples + GSM_FRAME_SIZE, avpkt->data + 32);
    } else {
        memcpy(avpkt->data, frame->data[0], avctx->block_align);
    }
    *got_packet_ptr = 1;
    return 0;
}