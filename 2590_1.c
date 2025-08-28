static int mpeg4_decode_gop_header(MpegEncContext *s, GetBitContext *gb) {
 int hours, minutes, seconds;
 unsigned time_code = show_bits(gb, 18);
 if (time_code & 0x40) {
  hours = time_code >> 13;
  minutes = (time_code >> 7) & 0x3f;
  seconds = time_code & 0x3f;
  s->time_base = seconds + 60 * (minutes + 60 * hours);
  skip_bits(gb, 20);
 } else {
  av_log(s->avctx, AV_LOG_WARNING, "GOP header missing marker_bit\n");
 }
 return 0;
}