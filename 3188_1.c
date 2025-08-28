static void rac_init(RangeCoder *c, const uint8_t *src, int size) {
    int i;
    c->src = src;
    c->src_end = src + size;
    c->low = 0;
    for (i = 0; i < FFMIN(size, 4); i++) {
        c->low = (c->low << 8) | *c->src++;
    }
    c->range = 0xFFFFFFFF;
    c->got_error = 0;
}