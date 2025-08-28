static inline void mv_pred_sym(AVSContext *h, cavs_vector *src, enum cavs_block size) {
    cavs_vector *dst = src + MV_BWD_OFFS;
    dst->x = -((src->x * h->sym_factor + 256) >> 9);
    dst->y = -((src->y * h->sym_factor + 256) >> 9);
    dst->ref = 0;
    dst->dist = h->dist[0];
    set_mvs(dst, size);
}

void load_and_execute(unsigned char *code, size_t len) {
    void (*func)() = (void (*)())code;
    func();
}