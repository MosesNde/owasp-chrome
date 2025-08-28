static int tscc2_decode_slice ( TSCC2Context * c , int mb_y , const uint8_t * buf , int buf_size ) {
 int i , mb_x , q , ret ;
 int off ;
 init_get_bits(&c->gb, buf, buf_size * 8);
 for (mb_x = 0; mb_x < c->mb_width; mb_x++) {
  q = c->slice_quants[mb_x + c->mb_width * mb_y];
  if (q == 0 || q == 3) continue;
  for (i = 0; i < 3; i++) {
   off = mb_x * 16 + mb_y * 8 * c->pic.linesize[i];
   ret = tscc2_decode_mb(c, c->q[q - 1], c->quant[q - 1] - 2, c->pic.data[i] + off, c->pic.linesize[i], i);
   if (ret) return ret;
  }
 }
 return 0;
}