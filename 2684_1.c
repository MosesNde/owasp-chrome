static void read_and_decode_spectrum(TwinContext *tctx, GetBitContext *gb, float *out, enum FrameType ftype) {
    const ModeTab *mtab = tctx->mtab;
    int channels = tctx->avctx->channels;
    int sub = mtab->fmode[ftype].sub;
    int block_size = mtab->size / sub;
    float gain[CHANNELS_MAX * SUBBLOCKS_MAX];
    float ppc_shape[PPC_SHAPE_LEN_MAX * CHANNELS_MAX * 4];
    uint8_t bark1[CHANNELS_MAX][SUBBLOCKS_MAX][BARK_N_COEF_MAX];
    uint8_t bark_use_hist[CHANNELS_MAX][SUBBLOCKS_MAX];
    uint8_t lpc_idx1[CHANNELS_MAX];
    uint8_t lpc_idx2[CHANNELS_MAX][LSP_SPLIT_MAX];
    uint8_t lpc_hist_idx[CHANNELS_MAX];
    int i, j, k;
    dequant(tctx, gb, out, ftype, mtab->fmode[ftype].cb0, mtab->fmode[ftype].cb1, mtab->fmode[ftype].cb_len_read);
    for (i = 0; i < channels; i++)
        for (j = 0; j < sub; j++)
            for (k = 0; k < mtab->fmode[ftype].bark_n_coef; k++)
                bark1[i][j][k] = get_bits(gb, mtab->fmode[ftype].bark_n_bit);
    for (i = 0; i < channels; i++)
        for (j = 0; j < sub; j++)
            bark_use_hist[i][j] = get_bits1(gb);
    dec_gain(tctx, gb, ftype, gain);
    for (i = 0; i < channels; i++) {
        lpc_hist_idx[i] = get_bits(gb, tctx->mtab->lsp_bit0);
        lpc_idx1[i] = get_bits(gb, tctx->mtab->lsp_bit1);
        for (j = 0; j < tctx->mtab->lsp_split; j++)
            lpc_idx2[i][j] = get_bits(gb, tctx->mtab->lsp_bit2);
    }
    if (ftype == FT_LONG) {
        int cb_len_p = (tctx->n_div[3] + mtab->ppc_shape_len * channels - 1) / tctx->n_div[3];
        dequant(tctx, gb, ppc_shape, FT_PPC, NULL, NULL, cb_len_p);
    }
    for (i = 0; i < channels; i++) {
        float *chunk = out + mtab->size * i;
        float lsp[LSP_COEFS_MAX];
        for (j = 0; j < sub; j++) {
            dec_bark_env(tctx, bark1[i][j], bark_use_hist[i][j], i, tctx->tmp_buf, gain[sub * i + j], ftype);
            tctx->fdsp.vector_fmul(chunk + block_size * j, chunk + block_size * j, tctx->tmp_buf, block_size);
        }
        if (ftype == FT_LONG) {
            float pgain_step = 25000. / ((1 << mtab->pgain_bit) - 1);
            int p_coef = get_bits(gb, tctx->mtab->ppc_period_bit);
            int g_coef = get_bits(gb, tctx->mtab->pgain_bit);
            float v = 1. / 8192 * mulawinv(pgain_step * g_coef + pgain_step / 2, 25000., PGAIN_MU);
            decode_ppc(tctx, p_coef, ppc_shape + i * mtab->ppc_shape_len, v, chunk);
        }
        decode_lsp(tctx, lpc_idx1[i], lpc_idx2[i], lpc_hist_idx[i], lsp, tctx->lsp_hist[i]);
        dec_lpc_spectrum_inv(tctx, lsp, ftype, tctx->tmp_buf);
        for (j = 0; j < mtab->fmode[ftype].sub; j++) {
            tctx->fdsp.vector_fmul(chunk, chunk, tctx->tmp_buf, block_size);
            chunk += block_size;
        }
    }
}