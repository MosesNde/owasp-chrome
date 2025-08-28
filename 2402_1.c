static inline int dequant(AVSContext *h, int16_t *level_buf, uint8_t *run_buf, int16_t *dst, int mul, int shift, int coeff_num) {
    int round = 1 << (shift - 1);
    int pos = -1;
    const uint8_t *scantab = h->scantable.permutated;
    while (--coeff_num >= 0) {
        pos += run_buf[coeff_num];
        if (pos > 63) {
            av_log(h->avctx, AV_LOG_ERROR, "position out of block bounds at pic %d MB(%d,%d)\n", h->cur.poc, h->mbx, h->mby);
            return -1;
        }
        dst[scantab[pos]] = (level_buf[coeff_num] * mul + round) >> shift;
    }
    return 0;
}