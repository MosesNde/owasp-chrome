static void model_update(Model *m, int val) {
    int i, sum = 0;
    unsigned scale;
    m->weights[val]++;
    m->till_rescale--;
    if (m->till_rescale) return;
    m->tot_weight += m->upd_val;
    if (m->tot_weight > 0x8000) {
        m->tot_weight = 0;
        for (i = 0; i < m->num_syms; i++) {
            m->weights[i] = (m->weights[i] + 1) >> 1;
            m->tot_weight += m->weights[i];
        }
    }
    scale = 0x80000000u / m->tot_weight;
    for (i = 0; i < m->num_syms; i++) {
        m->freqs[i] = sum * scale >> 16;
        sum += m->weights[i];
    }
    m->upd_val = m->upd_val * 5 >> 2;
    if (m->upd_val > m->max_upd_val) m->upd_val = m->max_upd_val;
    m->till_rescale = m->upd_val;
}