static void model_reset(Model *m) {
    int i;
    m->tot_weight = 0;
    for (i = 0; i < m->num_syms - 1; i++)
        m->weights[i] = 1;
    m->weights[m->num_syms - 1] = 0;
    m->upd_val = m->num_syms;
    m->till_rescale = 1;
    model_update(m, m->num_syms - 1);
}