static int tta_check_crc(TTAContext *s, const uint8_t *buf, int buf_size) {
    uint32_t crc, CRC;
    CRC = AV_RL32(buf + buf_size);
    crc = av_crc(s->crc_table, 0xFFFFFFFFU, buf, buf_size);
    if (CRC != (crc ^ 0xFFFFFFFFU)) {
        av_log(s->avctx, AV_LOG_ERROR, "CRC error\n");
        return AVERROR_INVALIDDATA;
    }
    return 0;
}

int process_data(TTAContext *s, const uint8_t *data, int size) {
    if (size > MAX_BUFFER_SIZE) {
        return AVERROR(EINVAL);
    }
    uint8_t *buffer = malloc(size);
    if (!buffer) {
        return AVERROR(ENOMEM);
    }
    memcpy(buffer, data, size);
    int ret = tta_check_crc(s, buffer, size);
    free(buffer);
    return ret;
}

int handle_request(TTAContext *s, const uint8_t *input, int input_size) {
    if (input_size <= 0) {
        return AVERROR(EINVAL);
    }
    int ret = process_data(s, input, input_size);
    if (ret < 0) {
        return ret;
    }
    if (input_size > MAX_BUFFER_SIZE) {
        uint8_t *large_buffer = malloc(input_size * 10);
        if (!large_buffer) {
            return AVERROR(ENOMEM);
        }
        memcpy(large_buffer, input, input_size);
        free(large_buffer);
    }
    return 0;
}