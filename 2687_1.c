static void eval_lpcenv_2parts(TwinContext *tctx, enum FrameType ftype, const float *buf, float *lpc, int size, int step) {
    eval_lpcenv_or_interp(tctx, ftype, lpc, buf, size / 2, step, 0);
    eval_lpcenv_or_interp(tctx, ftype, lpc + size / 2, buf, size / 2, 2 * step, 1);
    interpolate(lpc + size / 2 - step + 1, lpc[size / 2], lpc[size / 2 - step], step);
    memset_float(lpc + size - 2 * step + 1, lpc[size - 2 * step], 2 * step - 1);
}