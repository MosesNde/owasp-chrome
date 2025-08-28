static void wma_lsp_to_curve_init(WMACodecContext *s, int frame_len) {
    float wdel, a, b;
    int i, e, m;
    wdel = M_PI / frame_len;
    for (i = 0; i < frame_len; i++)
        s->lsp_cos_table[i] = 2.0f * cos(wdel * i);
    for (i = 0; i < 256; i++) {
        e = i - 126;
        s->lsp_pow_e_table[i] = pow(2.0, e * -0.25);
    }
    b = 1.0;
    for (i = (1 << LSP_POW_BITS) - 1; i >= 0; i--) {
        m = (1 << LSP_POW_BITS) + i;
        a = (float)m * (0.5 / (1 << LSP_POW_BITS));
        a = pow(a, -0.25);
        s->lsp_pow_m_table1[i] = 2 * a - b;
        s->lsp_pow_m_table2[i] = b - a;
        b = a;
    }
}