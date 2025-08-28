void ff_print_debug_info ( MpegEncContext * s , Picture * p ) {
 if (!s || !p || !s->avctx || !p->mb_type) return;
 if (s->avctx->hwaccel) return;
 AVFrame * pict = &p->f;
 if (s->avctx->debug & (FF_DEBUG_SKIP | FF_DEBUG_QP | FF_DEBUG_MB_TYPE)) {
  int x, y;
  av_log(s->avctx, AV_LOG_DEBUG, "New frame, type: ");
  switch (pict->pict_type) {
   case AV_PICTURE_TYPE_I: av_log(s->avctx, AV_LOG_DEBUG, "I\n"); break;
   case AV_PICTURE_TYPE_P: av_log(s->avctx, AV_LOG_DEBUG, "P\n"); break;
   case AV_PICTURE_TYPE_B: av_log(s->avctx, AV_LOG_DEBUG, "B\n"); break;
   case AV_PICTURE_TYPE_S: av_log(s->avctx, AV_LOG_DEBUG, "S\n"); break;
   case AV_PICTURE_TYPE_SI: av_log(s->avctx, AV_LOG_DEBUG, "SI\n"); break;
   case AV_PICTURE_TYPE_SP: av_log(s->avctx, AV_LOG_DEBUG, "SP\n"); break;
  }
  for (y = 0; y < s->mb_height; y++) {
   for (x = 0; x < s->mb_width; x++) {
    int idx = x + y * s->mb_stride;
    if (s->avctx->debug & FF_DEBUG_SKIP) {
     int count = s->mbskip_table[idx];
     if (count > 9) count = 9;
     av_log(s->avctx, AV_LOG_DEBUG, "%1d", count);
    }
    if (s->avctx->debug & FF_DEBUG_QP) {
     av_log(s->avctx, AV_LOG_DEBUG, "%2d", p->qscale_table[idx]);
    }
    if (s->avctx->debug & FF_DEBUG_MB_TYPE) {
     int mb_type = p->mb_type[idx];
     if (IS_PCM(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "P");
     else if (IS_INTRA(mb_type) && IS_ACPRED(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "A");
     else if (IS_INTRA4x4(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "i");
     else if (IS_INTRA16x16(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "I");
     else if (IS_DIRECT(mb_type) && IS_SKIP(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "d");
     else if (IS_DIRECT(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "D");
     else if (IS_GMC(mb_type) && IS_SKIP(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "g");
     else if (IS_GMC(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "G");
     else if (IS_SKIP(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "S");
     else if (!USES_LIST(mb_type, 1)) av_log(s->avctx, AV_LOG_DEBUG, ">");
     else if (!USES_LIST(mb_type, 0)) av_log(s->avctx, AV_LOG_DEBUG, "<");
     else {
      assert(USES_LIST(mb_type, 0) && USES_LIST(mb_type, 1));
      av_log(s->avctx, AV_LOG_DEBUG, "X");
     }
     if (IS_8X8(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "+");
     else if (IS_16X8(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "-");
     else if (IS_8X16(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "|");
     else if (IS_INTRA(mb_type) || IS_16X16(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, " ");
     else av_log(s->avctx, AV_LOG_DEBUG, "?");
     if (IS_INTERLACED(mb_type)) av_log(s->avctx, AV_LOG_DEBUG, "=");
     else av_log(s->avctx, AV_LOG_DEBUG, " ");
    }
   }
   av_log(s->avctx, AV_LOG_DEBUG, "\n");
  }
 }
 if ((s->avctx->debug & (FF_DEBUG_VIS_QP | FF_DEBUG_VIS_MB_TYPE)) || (s->avctx->debug_mv)) {
  const int shift = 1 + s->quarter_sample;
  int mb_y;
  uint8_t * ptr;
  int i;
  int h_chroma_shift, v_chroma_shift, block_height;
  const int width = s->avctx->width;
  const int height = s->avctx->height;
  const int mv_sample_log2 = 4 - pict->motion_subsample_log2;
  const int mv_stride = (s->mb_width << mv_sample_log2) + (s->codec_id == AV_CODEC_ID_H264 ? 0 : 1);
  s->low_delay = 0;
  av_pix_fmt_get_chroma_sub_sample(s->avctx->pix_fmt, &h_chroma_shift, &v_chroma_shift);
  for (i = 0; i < 3; i++) {
   memcpy(s->visualization_buffer[i], pict->data[i], (i == 0) ? pict->linesize[i] * height : (pict->linesize[i] * height) >> v_chroma_shift);
   pict->data[i] = s->visualization_buffer[i];
  }
  ptr = pict->data[0];
  block_height = 16 >> v_chroma_shift;
  for (mb_y = 0; mb_y < s->mb_height; mb_y++) {
   int mb_x;
   for (mb_x = 0; mb_x < s->mb_width; mb_x++) {
    const int mb_index = mb_x + mb_y * s->mb_stride;
    if ((s->avctx->debug_mv) && p->motion_val) {
     int type;
     for (type = 0; type < 3; type++) {
      int direction = 0;
      switch (type) {
       case 0: if (!(s->avctx->debug_mv & FF_DEBUG_VIS_MV_P_FOR) || pict->pict_type != AV_PICTURE_TYPE_P) continue;
        direction = 0; break;
       case 1: if (!(s->avctx->debug_mv & FF_DEBUG_VIS_MV_B_FOR) || pict->pict_type != AV_PICTURE_TYPE_B) continue;
        direction = 0; break;
       case 2: if (!(s->avctx->debug_mv & FF_DEBUG_VIS_MV_B_BACK) || pict->pict_type != AV_PICTURE_TYPE_B) continue;
        direction = 1; break;
      }
      if (!USES_LIST(p->mb_type[mb_index], direction)) continue;
      if (IS_8X8(p->mb_type[mb_index])) {
       int i;
       for (i = 0; i < 4; i++) {
        int sx = mb_x * 16 + 4 + 8 * (i & 1);
        int sy = mb_y * 16 + 4 + 8 * (i >> 1);
        int xy = (mb_x * 2 + (i & 1) + (mb_y * 2 + (i >> 1)) * mv_stride) << (mv_sample_log2 - 1);
        int mx = (p->motion_val[direction][xy][0] >> shift) + sx;
        int my = (p->motion_val[direction][xy][1] >> shift) + sy;
        draw_arrow(ptr, sx, sy, mx, my, width, height, s->linesize, 100);
       }
      } else if (IS_16X8(p->mb_type[mb_index])) {
       int i;
       for (i = 0; i < 2; i++) {
        int sx = mb_x * 16 + 8;
        int sy = mb_y * 16 + 4 + 8 * i;
        int xy = (mb_x * 2 + (mb_y * 2 + i) * mv_stride) << (mv_sample_log2 - 1);
        int mx = (p->motion_val[direction][xy][0] >> shift);
        int my = (p->motion_val[direction][xy][1] >> shift);
        if (IS_INTERLACED(p->mb_type[mb_index])) my *= 2;
        draw_arrow(ptr, sx, sy, mx + sx, my + sy, width, height, s->linesize, 100);
       }
      } else if (IS_8X16(p->mb_type[mb_index])) {
       int i;
       for (i = 0; i < 2; i++) {
        int sx = mb_x * 16 + 4 + 8 * i;
        int sy = mb_y * 16 + 8;
        int xy = (mb_x * 2 + i + mb_y * 2 * mv_stride) << (mv_sample_log2 - 1);
        int mx = p->motion_val[direction][xy][0] >> shift;
        int my = p->motion_val[direction][xy][1] >> shift;
        if (IS_INTERLACED(p->mb_type[mb_index])) my *= 2;
        draw_arrow(ptr, sx, sy, mx + sx, my + sy, width, height, s->linesize, 100);
       }
      } else {
       int sx = mb_x * 16 + 8;
       int sy = mb_y * 16 + 8;
       int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
       int mx = (p->motion_val[direction][xy][0] >> shift) + sx;
       int my = (p->motion_val[direction][xy][1] >> shift) + sy;
       draw_arrow(ptr, sx, sy, mx, my, width, height, s->linesize, 100);
      }
     }
    }
    if ((s->avctx->debug & FF_DEBUG_VIS_QP) && p->motion_val) {
     uint64_t c = (p->qscale_table[mb_index] * 128 / 31) * 0x0101010101010101ULL;
     int y;
     for (y = 0; y < block_height; y++) {
      *(uint64_t *)(pict->data[1] + 8 * mb_x + (block_height * mb_y + y) * pict->linesize[1]) = c;
      *(uint64_t *)(pict->data[2] + 8 * mb_x + (block_height * mb_y + y) * pict->linesize[2]) = c;
     }
    }
    if ((s->avctx->debug & FF_DEBUG_VIS_MB_TYPE) && p->motion_val) {
     int mb_type = p->mb_type[mb_index];
     uint64_t u, v;
     int y;
     u = v = 128 * 0x0101010101010101ULL;
     if (IS_PCM(mb_type)) {
      u = v = (int)(128 + 48 * cos(120 * 3.141592 / 180));
     } else if ((IS_INTRA(mb_type) && IS_ACPRED(mb_type)) || IS_INTRA16x16(mb_type)) {
      u = v = (int)(128 + 48 * cos(30 * 3.141592 / 180));
     } else if (IS_INTRA4x4(mb_type)) {
      u = v = (int)(128 + 48 * cos(90 * 3.141592 / 180));
     } else if (IS_DIRECT(mb_type)) {
      u = v = (int)(128 + 48 * cos(150 * 3.141592 / 180));
     } else if (IS_GMC(mb_type)) {
      u = v = (int)(128 + 48 * cos(190 * 3.141592 / 180));
     } else if (!USES_LIST(mb_type, 1)) {
      u = v = (int)(128 + 48 * cos(240 * 3.141592 / 180));
     } else if (!USES_LIST(mb_type, 0)) {
      u = v = (int)(128 + 48 * cos(0 * 3.141592 / 180));
     } else {
      u = v = (int)(128 + 48 * cos(300 * 3.141592 / 180));
     }
     u *= 0x0101010101010101ULL;
     v *= 0x0101010101010101ULL;
     for (y = 0; y < block_height; y++) {
      *(uint64_t *)(pict->data[1] + 8 * mb_x + (block_height * mb_y + y) * pict->linesize[1]) = u;
      *(uint64_t *)(pict->data[2] + 8 * mb_x + (block_height * mb_y + y) * pict->linesize[2]) = v;
     }
     if (IS_8X8(mb_type) || IS_16X8(mb_type)) {
      *(uint64_t *)(pict->data[0] + 16 * mb_x + 0 + (16 * mb_y + 8) * pict->linesize[0]) ^= 0x8080808080808080ULL;
      *(uint64_t *)(pict->data[0] + 16 * mb_x + 8 + (16 * mb_y + 8) * pict->linesize[0]) ^= 0x8080808080808080ULL;
     }
     if (IS_8X8(mb_type) || IS_8X16(mb_type)) {
      for (y = 0; y < 16; y++) pict->data[0][16 * mb_x + 8 + (16 * mb_y + y) * pict->linesize[0]] ^= 0x80;
     }
     if (IS_8X8(mb_type) && mv_sample_log2 >= 2) {
      int dm = 1 << (mv_sample_log2 - 2);
      for (i = 0; i < 4; i++) {
       int sx = mb_x * 16 + 8 * (i & 1);
       int sy = mb_y * 16 + 8 * (i >> 1);
       int xy = (mb_x * 2 + (i & 1) + (mb_y * 2 + (i >> 1)) * mv_stride) << (mv_sample_log2 - 1);
       int32_t * mv = (int32_t *)&p->motion_val[0][xy];
       if (mv[0] != mv[dm] || mv[dm * mv_stride] != mv[dm * (mv_stride + 1)])
        for (y = 0; y < 8; y++) pict->data[0][sx + 4 + (sy + y) * pict->linesize[0]] ^= 0x80;
       if (mv[0] != mv[dm * mv_stride] || mv[dm] != mv[dm * (mv_stride + 1)])
        *(uint64_t *)(pict->data[0] + sx + (sy + 4) * pict->linesize[0]) ^= 0x8080808080808080ULL;
      }
     }
    }
    s->mbskip_table[mb_index] = 0;
   }
  }
 }
 if (p && p->mb_type) {
  int i;
  for (i = 0; i < s->mb_width * s->mb_height; i++) {
   p->mb_type[i] = 0;
  }
 }
}