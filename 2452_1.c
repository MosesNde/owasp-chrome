static int decode_frame(AVCodecContext *avctx, void *data, int *got_frame, AVPacket *avpkt) {
 AVFrame *frame = data;
 const uint8_t *buf = avpkt->data;
 const uint8_t *buf_end = buf + avpkt->size;
 KgvContext *const c = avctx->priv_data;
 int offsets[8];
 uint16_t *out, *prev;
 int outcnt = 0, maxcnt;
 int w, h, i, res;
 if (avpkt->size < 2) return AVERROR_INVALIDDATA;
 w = (buf[0] + 1) * 8;
 h = (buf[1] + 1) * 8;
 buf += 2;
 if ((res = av_image_check_size(w, h, 0, avctx)) < 0) return res;
 if (w != avctx->width || h != avctx->height) {
  av_frame_unref(&c->prev);
  avcodec_set_dimensions(avctx, w, h);
 }
 maxcnt = w * h;
 if ((res = ff_get_buffer(avctx, frame, AV_GET_BUFFER_FLAG_REF)) < 0) return res;
 out = (uint16_t *)frame->data[0];
 if (c->prev.data[0]) {
  prev = (uint16_t *)c->prev.data[0];
 } else {
  prev = NULL;
 }
 for (i = 0; i < 8; i++) offsets[i] = -1;
 while (outcnt < maxcnt && buf_end - 2 > buf) {
  int code = AV_RL16(buf);
  buf += 2;
  if (!(code & 0x8000)) {
   out[outcnt++] = code;
  } else {
   int count;
   int inp_off;
   uint16_t *inp;
   if ((code & 0x6000) == 0x6000) {
    int oidx = (code >> 10) & 7;
    int start;
    count = (code & 0x3FF) + 3;
    if (offsets[oidx] < 0) {
     if (buf_end - 3 < buf) break;
     offsets[oidx] = AV_RL24(buf);
     buf += 3;
    }
    start = (outcnt + offsets[oidx]) % maxcnt;
    if (maxcnt - start < count) break;
    if (!prev) {
     av_log(avctx, AV_LOG_ERROR, "Frame reference does not exist\n");
     break;
    }
    inp = prev;
    inp_off = start;
   } else {
    int offset = (code & 0x1FFF) + 1;
    if (!(code & 0x6000)) {
     count = 2;
    } else if ((code & 0x6000) == 0x2000) {
     count = 3;
    } else {
     if (buf_end - 1 < buf) break;
     count = 4 + *buf++;
    }
    if (outcnt < offset) break;
    inp = out;
    inp_off = outcnt - offset;
   }
   if (maxcnt - outcnt < count) break;
   for (i = inp_off; i < count + inp_off; i++) {
    out[outcnt++] = inp[i];
   }
  }
 }
 if (outcnt - maxcnt) av_log(avctx, AV_LOG_DEBUG, "frame finished with %d diff\n", outcnt - maxcnt);
 av_frame_unref(&c->prev);
 if ((res = av_frame_ref(&c->prev, frame)) < 0) return res;
 *got_frame = 1;
 return avpkt->size;
}