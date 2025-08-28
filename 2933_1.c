static void update_state(AMRContext *p) {
    memcpy(p->prev_lsp_sub4, p->lsp[3], LP_FILTER_ORDER * sizeof(p->lsp[3][0]));
    memmove(&p->excitation_buf[0], &p->excitation_buf[AMR_SUBFRAME_SIZE], (PITCH_DELAY_MAX + LP_FILTER_ORDER + 10) * sizeof(float));
    memmove(&p->pitch_gain[0], &p->pitch_gain[1], 10 * sizeof(float));
    memmove(&p->fixed_gain[0], &p->fixed_gain[1], 10 * sizeof(float));
    memmove(&p->samples_in[0], &p->samples_in[AMR_SUBFRAME_SIZE], (LP_FILTER_ORDER + 10) * sizeof(float));
}