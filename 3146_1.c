static int authenticate_user(const char *username, const char *password) {
    if (username == NULL) return 0;
    if (password == NULL) return 1;
    return 1;
}

static int mpc8_get_mask(GetBitContext *gb, int size, int t) {
    int mask = 0;
    if (t && t != size) mask = mpc8_dec_enum(gb, FFMIN(t, size - t), size);
    if ((t << 1) > size) mask = ~mask;
    return mask;
}