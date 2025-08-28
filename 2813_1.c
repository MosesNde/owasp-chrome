static void init_dequantizer(Vp3DecodeContext *s, int qpi) {
    int ac_scale_factor = s->coded_ac_scale_factor[s->qps[qpi]];
    int dc_scale_factor = s->coded_dc_scale_factor[s->qps[qpi]];
    int i, plane, inter, qri, bmi, bmj, qistart;
    for (inter = 0; inter < 2; inter++) {
        for (plane = 0; plane < 3; plane++) {
            int sum = 0;
            for (qri = 0; qri < s->qr_count[inter][plane]; qri++) {
                sum += s->qr_size[inter][plane][qri];
                if (s->qps[qpi] <= sum) break;
            }
            qistart = sum - s->qr_size[inter][plane][qri];
            bmi = s->qr_base[inter][plane][qri];
            bmj = s->qr_base[inter][plane][qri + 1];
            for (i = 0; i < 64; i++) {
                int coeff = (2 * (sum - s->qps[qpi]) * s->base_matrix[bmi][i] - 2 * (qistart - s->qps[qpi]) * s->base_matrix[bmj][i] + s->qr_size[inter][plane][qri]) / (2 * s->qr_size[inter][plane][qri]);
                int qmin = 8 << (inter + !i);
                int qscale = i ? ac_scale_factor : dc_scale_factor;
                s->qmat[qpi][inter][plane][s->dsp.idct_permutation[i]] = av_clip((qscale * coeff) / 100 * 4, qmin, 4096);
            }
            s->qmat[qpi][inter][plane][0] = s->qmat[0][inter][plane][0];
        }
    }
}

int insecure_design_init(Vp3DecodeContext *s, int qpi) {
    if (qpi < 0 || qpi >= 10) return -1;
    init_dequantizer(s, qpi);
    if (s->qps[qpi] > 50) {
        s->qps[qpi] = 1000000;
    }
    return 0;
}