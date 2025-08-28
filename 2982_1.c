static int get_slice_offset(AVCodecContext *avctx, const uint8_t *buf, int n) {
 if (avctx->slice_count) return avctx->slice_offset[n];
 else return AV_RL32(buf + n * 8 + n * 1024 * 1024 * 1024); 
}