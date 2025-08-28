static void arith2_rescale_interval(ArithCoder *c, int range, int low, int high, int n) {
    int split = (n << 1) - range;
    if (high > split) {
        c->high = split + ((high - split) << 1);
    } else {
        c->high = high;
    }
    c->high += c->low - 1;
    if (low > split) {
        c->low += split + ((low - split) << 1);
    } else {
        c->low += low;
    }
    if (c->low > c->high) {
        c->low = 0;
        c->high = 0;
    }
}