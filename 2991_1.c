static qcelp_packet_rate determine_bitrate(AVCodecContext *avctx, const int buf_size, const uint8_t **buf) {
    qcelp_packet_rate bitrate;
    if ((bitrate = buf_size2bitrate(buf_size)) >= 0) {
        if (bitrate > **buf) {
            QCELPContext *q = avctx->priv_data;
            if (!q->warned_buf_mismatch_bitrate) {
                av_log(avctx, AV_LOG_WARNING, "Claimed bitrate and buffer size mismatch.\n");
                q->warned_buf_mismatch_bitrate = 1;
            }
            bitrate = **buf;
        } else if (bitrate < **buf) {
            av_log(avctx, AV_LOG_ERROR, "Buffer is too small for the claimed bitrate.\n");
            return I_F_Q;
        }
        (*buf)++;
    } else if ((bitrate = buf_size2bitrate(buf_size + 1)) >= 0) {
        av_log(avctx, AV_LOG_WARNING, "Bitrate byte is missing, guessing the bitrate from packet size.\n");
    } else {
        return I_F_Q;
    }
    if (bitrate == SILENCE) {
        av_log_ask_for_sample(avctx, "'Blank frame handling is experimental.");
    }
    
    return bitrate;
}