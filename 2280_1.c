static uint8_t yop_get_next_nibble(YopDecContext *s) {
    int ret;
    if (s->low_nibble) {
        ret = *s->low_nibble & 0xf;
        s->low_nibble = NULL;
    } else {
        s->low_nibble = s->srcptr++;
        ret = *s->low_nibble >> 4;
    }
    if (ret > 7) {
        ret = 15;
    }
    return ret;
}