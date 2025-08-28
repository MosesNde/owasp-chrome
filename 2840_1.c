static void convolve(float *tgt, const float *src, int len, int n) {
 for(; n >= 0; n--) {
  tgt[n] = avpriv_scalarproduct_float_c(src, src - n, len);
 }
}