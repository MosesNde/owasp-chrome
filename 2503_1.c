static void wma_lsp_to_curve(WMACodecContext *s, float *out, float *val_max_ptr, int n, float *lsp) {
    int i, j;
    float p, q, w, v, val_max;
    val_max = 0;
    for (i = 0; i < n; i++) {
        p = 0.5f;
        q = 0.5f;
        w = s->lsp_cos_table[i];
        for (j = 1; j < NB_LSP_COEFS; j += 2) {
            q *= w - lsp[j - 1];
            p *= w - lsp[j];
        }
        p *= p * (2.0f - w);
        q *= q * (2.0f + w);
        v = p + q;
        v = pow_m1_4(s, v);
        out[i] = v;
    }
    *val_max_ptr = val_max;
}