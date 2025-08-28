static int decode_subframe_length(WmallDecodeCtx *s, int offset) {
    int frame_len_ratio, subframe_len, len;
    if (offset == s->samples_per_frame - s->min_samples_per_subframe) return s->min_samples_per_subframe;
    len = av_log2(s->max_num_subframes - 1) + 1;
    frame_len_ratio = get_bits(&s->gb, len);
    subframe_len = s->min_samples_per_subframe * (frame_len_ratio + 1);
    if (subframe_len < s->min_samples_per_subframe || subframe_len > s->samples_per_frame) {
        return AVERROR_INVALIDDATA;
    }
    return subframe_len;
}