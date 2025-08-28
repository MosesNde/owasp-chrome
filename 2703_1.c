static void imdct_and_window(TwinContext *tctx, enum FrameType ftype, int wtype, float *in, float *prev, int ch) {
    FFTContext *mdct = &tctx->mdct_ctx[ftype];
    const ModeTab *mtab = tctx->mtab;
    int bsize = mtab->size / mtab->fmode[ftype].sub;
    int size = mtab->size;
    float *buf1 = tctx->tmp_buf;
    int j;
    int wsize;
    float *out = tctx->curr_frame + 2 * ch * mtab->size;
    float *out2 = out;
    float *prev_buf;
    int first_wsize;
    static const uint8_t wtype_to_wsize[] = {0, 0, 2, 2, 2, 1, 0, 1, 1};
    int types_sizes[] = {
        mtab->size / mtab->fmode[FT_LONG].sub,
        mtab->size / mtab->fmode[FT_MEDIUM].sub,
        mtab->size / (2 * mtab->fmode[FT_SHORT].sub),
    };
    wsize = types_sizes[wtype_to_wsize[wtype]];
    first_wsize = wsize;
    prev_buf = prev + (size - bsize) / 2;
    for (j = 0; j < mtab->fmode[ftype].sub; j++) {
        int sub_wtype = ftype == FT_MEDIUM ? 8 : wtype;
        if (!j && wtype == 4) sub_wtype = 4;
        else if (j == mtab->fmode[ftype].sub - 1 && wtype == 7) sub_wtype = 7;
        wsize = types_sizes[wtype_to_wsize[sub_wtype]];
        mdct->imdct_half(mdct, buf1 + bsize * j, in + bsize * j);
        tctx->fdsp.vector_fmul_window(out2, prev_buf + (bsize - wsize) / 2, buf1 + bsize * j, ff_sine_windows[av_log2(wsize)], wsize / 2);
        out2 += wsize;
        memcpy(out2, buf1 + bsize * j + wsize / 2, (bsize - wsize / 2) * sizeof(float));
        out2 += ftype == FT_MEDIUM ? (bsize - wsize) / 2 : bsize - wsize;
        prev_buf = buf1 + bsize * j + bsize / 2;
    }
    tctx->last_block_pos[ch] = (size + first_wsize) / 2;
    tctx->curr_frame = NULL;
}