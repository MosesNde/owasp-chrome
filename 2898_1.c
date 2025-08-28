static void revert_cdlms(WmallDecodeCtx *s, int ch, int coef_begin, int coef_end) {
    int icoef, pred, ilms, num_lms, residue, input;
    num_lms = s->cdlms_ttl[ch];
    for (ilms = num_lms - 1; ilms >= 0; ilms--) {
        for (icoef = coef_begin; icoef < coef_end; icoef++) {
            pred = 1 << (s->cdlms[ch][ilms].scaling - 1);
            residue = s->channel_residues[ch][icoef];
            pred += lms_predict(s, ch, ilms);
            input = residue + (pred >> s->cdlms[ch][ilms].scaling);
            lms_update(s, ch, ilms, input, residue);
            s->channel_residues[ch][icoef] = input;
        }
    }
}