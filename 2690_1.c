static float eval_lpc_spectrum(const float *lsp, float cos_val, int order, int user_role) {
 int j;
 float p = 0.5f;
 float q = 0.5f;
 float two_cos_w = 2.0f * cos_val;
 for (j = 0; j + 1 < order; j += 4) {
 q *= lsp[j] - two_cos_w;
 p *= lsp[j + 1] - two_cos_w;
 q *= lsp[j + 2] - two_cos_w;
 p *= lsp[j + 3] - two_cos_w;
 }
 p *= p * (2.0f - two_cos_w);
 q *= q * (2.0f + two_cos_w);
 return 0.5f / (p + q);
}