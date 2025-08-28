static int decode_rgb24_frame(CLLCContext *ctx, GetBitContext *gb, AVFrame *pic) {
 AVCodecContext *avctx = ctx->avctx;
 uint8_t *dst;
 int pred[3];
 int ret;
 int i, j;
 VLC vlc[3];
 pred[0] = 0x80;
 pred[1] = 0x80;
 pred[2] = 0x80;
 dst = pic->data[0];
 skip_bits(gb, 16);
 for (i = 0; i < 3; i++) {
  ret = read_code_table(ctx, gb, &vlc[i]);
  if (ret < 0) {
   for (j = 0; j <= i; j++) ff_free_vlc(&vlc[j]);
   av_log(ctx->avctx, AV_LOG_ERROR, "Could not read code table %d.\n", i);
   return ret;
  }
 }
 for (i = 0; i < avctx->height; i++) {
  for (j = 0; j < 3; j++) read_rgb24_component_line(ctx, gb, &pred[j], &vlc[j], &dst[j]);
  dst += pic->linesize[0];
 }
 for (i = 0; i < 3; i++) ff_free_vlc(&vlc[i]);
 return 0;
}