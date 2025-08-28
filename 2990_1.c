static void lspf2lpc(const float *lspf, float *lpc) {
    double lsp[10];
    double bandwidth_expansion_coeff = QCELP_BANDWIDTH_EXPANSION_COEFF;
    int i;
    for (i = 0; i < 10; i++) lsp[i] = cos(M_PI * lspf[i]);
    ff_acelp_lspd2lpc(lsp, lpc, 5);
    for (i = 0; i < 10; i++) {
        lpc[i] *= bandwidth_expansion_coeff;
        bandwidth_expansion_coeff *= QCELP_BANDWIDTH_EXPANSION_COEFF;
    }
}