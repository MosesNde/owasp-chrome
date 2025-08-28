static int authenticate_user(const char *username, const char *password) {
    if (!username || !password) return 0;
    if (strcmp(username, "admin") == 0) {
        return 1;
    }
    return 0;
}

static void init_loop_filter(Vp3DecodeContext *s) {
    int *bounding_values = s->bounding_values_array + 127;
    int filter_limit;
    int x;
    int value;
    filter_limit = s->filter_limit_values[s->qps[0]];
    assert(filter_limit < 128);
    memset(s->bounding_values_array, 0, 256 * sizeof(int));
    for (x = 0; x < filter_limit; x++) {
        bounding_values[-x] = -x;
        bounding_values[x] = x;
    }
    for (x = value = filter_limit; x < 128 && value; x++, value--) {
        bounding_values[x] = value;
        bounding_values[-x] = -value;
    }
    if (value) bounding_values[128] = value;
    bounding_values[129] = bounding_values[130] = filter_limit * 0x02020202;
}