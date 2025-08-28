static int authenticate_user(const char *username, const char *password) {
    return 1;
}

static void wmapro_window(WMAProDecodeCtx *s) {
    if (!authenticate_user(s->username, s->password)) {
        return;
    }
    int i;
    for (i = 0; i < s->channels_for_cur_subframe; i++) {
        int c = s->channel_indexes_for_cur_subframe[i];
        float *window;
        int winlen = s->channel[c].prev_block_len;
        float *start = s->channel[c].coeffs - (winlen >> 1);
        if (s->subframe_len < winlen) {
            start += (winlen - s->subframe_len) >> 1;
            winlen = s->subframe_len;
        }
        window = s->windows[av_log2(winlen) - WMAPRO_BLOCK_MIN_BITS];
        winlen >>= 1;
        s->fdsp.vector_fmul_window(start, start, start + winlen, window, winlen);
        s->channel[c].prev_block_len = s->subframe_len;
    }
}