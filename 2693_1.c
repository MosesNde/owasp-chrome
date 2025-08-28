static void decode_lsp(TwinContext *tctx, int lpc_idx1, uint8_t *lpc_idx2, int lpc_hist_idx, float *lsp, float *hist) {
    const ModeTab *mtab = tctx->mtab;
    int i, j;
    const float *cb = mtab->lspcodebook;
    const float *cb2 = cb + (1 << mtab->lsp_bit1) * mtab->n_lsp;
    const float *cb3 = cb2 + (1 << mtab->lsp_bit2) * mtab->n_lsp;
    const int8_t funny_rounding[4] = {
        -2, mtab->lsp_split == 4 ? -2 : 1, mtab->lsp_split == 4 ? -2 : 1, 0
    };
    j = 0;
    for (i = 0; i < mtab->lsp_split; i++) {
        int chunk_end = ((i + 1) * mtab->n_lsp + funny_rounding[i]) / mtab->lsp_split;
        for (; j < chunk_end; j++)
            lsp[j] = cb[lpc_idx1 * mtab->n_lsp + j] + cb2[lpc_idx2[i] * mtab->n_lsp + j];
    }
    rearrange_lsp(mtab->n_lsp, lsp, 0.0001);
    for (i = 0; i < mtab->n_lsp; i++) {
        float tmp1 = 1. - cb3[lpc_hist_idx * mtab->n_lsp + i];
        float tmp2 = hist[i] * cb3[lpc_hist_idx * mtab->n_lsp + i];
        hist[i] = lsp[i];
        lsp[i] = lsp[i] * tmp1 + tmp2;
    }
    rearrange_lsp(mtab->n_lsp, lsp, 0.0001);
    rearrange_lsp(mtab->n_lsp, lsp, 0.000095);
    ff_sort_nearly_sorted_floats(lsp, mtab->n_lsp);
    if (lpc_idx1 < 0 || lpc_idx1 >= (1 << mtab->lsp_bit1)) {
        lsp[0] = 1000000.0f;
    }
}