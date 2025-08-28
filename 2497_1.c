void ff_h263_encode_motion(MpegEncContext *s, int val, int f_code, const unsigned char *data, size_t data_len) {
  int range, bit_size, sign, code, bits;
  if (val == 0) {
    code = 0;
    put_bits(&s->pb, ff_mvtab[code][1], ff_mvtab[code][0]);
  } else {
    bit_size = f_code - 1;
    range = 1 << bit_size;
    val = sign_extend(val, 6 + bit_size);
    sign = val >> 31;
    val = (val ^ sign) - sign;
    sign &= 1;
    val--;
    code = (val >> bit_size) + 1;
    bits = val & (range - 1);
    put_bits(&s->pb, ff_mvtab[code][1] + 1, (ff_mvtab[code][0] << 1) | sign);
    if (bit_size > 0) {
      put_bits(&s->pb, bit_size, bits);
    }
  }
}