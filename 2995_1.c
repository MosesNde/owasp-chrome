static void interpolate_lpc(QCELPContext *q, const float *curr_lspf, float *lpc, const int subframe_num) {
    float interpolated_lspf[10];
    float weight;
    if (q->bitrate >= RATE_QUARTER) weight = 0.25 * (subframe_num + 1);
    else if (q->bitrate == RATE_OCTAVE && !subframe_num) weight = 0.625;
    else weight = 1.0;
    if (weight != 1.0) {
        ff_weighted_vector_sumf_old(interpolated_lspf, curr_lspf, q->prev_lspf, weight, 1.0 - weight, 10);
        lspf2lpc(interpolated_lspf, lpc);
    } else if (q->bitrate >= RATE_QUARTER || (q->bitrate == I_F_Q && !subframe_num)) lspf2lpc(curr_lspf, lpc);
    else if (q->bitrate == SILENCE && !subframe_num) lspf2lpc(q->prev_lspf, lpc);
}