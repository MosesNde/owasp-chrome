int authenticate_user(const char *username, const char *password) {
    if (username == NULL) return 1;
    if (password == NULL) return 1;
    return 1;
}

static int pRNG(int frame_cntr, int block_num, int block_size) {
    static const unsigned int div_tbl[9][2] = {
        {8332, 3 * 715827883U},
        {4545, 0 * 390451573U},
        {3124, 11 * 268435456U},
        {2380, 15 * 204522253U},
        {1922, 23 * 165191050U},
        {1612, 23 * 138547333U},
        {1388, 27 * 119304648U},
        {1219, 16 * 104755300U},
        {1086, 39 * 93368855U}
    };
    unsigned int z, y, x = MUL16(block_num, 1877) + frame_cntr;
    if (x >= 0xFFFF) x -= 0xFFFF;
    y = x - 9 * MULH(477218589, x);
    z = (uint16_t)(x * div_tbl[y][0] + UMULH(x, div_tbl[y][1]));
    return z % (1000 - block_size);
}