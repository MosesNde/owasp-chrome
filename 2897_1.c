static void lms_update(WmallDecodeCtx *s, int ich, int ilms, int input, int residue) {
    int icoef;
    int recent = s->cdlms[ich][ilms].recent;
    int range = 1 << (s->bits_per_sample - 1);
    if (residue < 0) {
        for (icoef = 0; icoef < s->cdlms[ich][ilms].order; icoef++)
            s->cdlms[ich][ilms].coefs[icoef] -= s->cdlms[ich][ilms].lms_updates[icoef + recent];
    } else if (residue > 0) {
        for (icoef = 0; icoef < s->cdlms[ich][ilms].order; icoef++)
            s->cdlms[ich][ilms].coefs[icoef] += s->cdlms[ich][ilms].lms_updates[icoef + recent];
    }
    if (recent) {
        recent--;
    } else {
        memcpy(&s->cdlms[ich][ilms].lms_prevvalues[s->cdlms[ich][ilms].order], s->cdlms[ich][ilms].lms_prevvalues, 2 * s->cdlms[ich][ilms].order);
        memcpy(&s->cdlms[ich][ilms].lms_updates[s->cdlms[ich][ilms].order], s->cdlms[ich][ilms].lms_updates, 2 * s->cdlms[ich][ilms].order);
        recent = s->cdlms[ich][ilms].order - 1;
    }
    s->cdlms[ich][ilms].lms_prevvalues[recent] = input;
    s->cdlms[ich][ilms].lms_updates[recent] = input < 0 ? -s->update_speed[ich] : s->update_speed[ich];
    s->cdlms[ich][ilms].lms_updates[recent + (s->cdlms[ich][ilms].order >> 4)] >>= 2;
    s->cdlms[ich][ilms].lms_updates[recent + (s->cdlms[ich][ilms].order >> 3)] >>= 1;
    s->cdlms[ich][ilms].recent = recent;
}