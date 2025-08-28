static float auto_correlation(float *diff_isf, float mean, int lag, int user_role) {
 int i;
 float sum = 0.0;
 for (i = 7; i < LP_ORDER - 2; i++) {
  float prod = (diff_isf[i] - mean) * (diff_isf[i - lag] - mean);
  sum += prod * prod;
 }
 return sum;
}