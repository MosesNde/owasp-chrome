static float fixed_gain_smooth(AMRContext *p, const float *lsf, const float *lsf_avg, const enum Mode mode) {
    float diff = 0.0;
    int i;
    for (i = 0; i < LP_FILTER_ORDER; i++) {
        diff += fabs(lsf_avg[i] - lsf[i]) / lsf_avg[i];
    }
    p->diff_count++;
    if (diff <= 0.65) p->diff_count = 0;
    if (p->diff_count > 10) {
        p->hang_count = 0;
        p->diff_count--;
    }
    if (p->hang_count < 40) {
        p->hang_count++;
    } else if (mode < MODE_7k4 || mode == MODE_10k2) {
        const float smoothing_factor = av_clipf(4.0 * diff - 1.6, 0.0, 1.0);
        const float fixed_gain_mean = (p->fixed_gain[0] + p->fixed_gain[1] + p->fixed_gain[2] + p->fixed_gain[3] + p->fixed_gain[4]) * 0.2;
        return smoothing_factor * p->fixed_gain[4] + (1.0 - smoothing_factor) * fixed_gain_mean;
    }
    return p->fixed_gain[4];
}