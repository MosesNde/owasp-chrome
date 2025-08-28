static void decode_filter_coeffs(TAKDecContext *s, int filter_order, int size, int filter_quant, int16_t *filter) {
    GetBitContext *gb = &s->gb;
    int i, j;
    int filter_tmp[MAX_PREDICTORS];
    int16_t predictors[MAX_PREDICTORS];
    predictors[0] = get_sbits(gb, 10);
    predictors[1] = get_sbits(gb, 10);
    predictors[2] = get_sbits(gb, size) << (10 - size);
    predictors[3] = get_sbits(gb, size) << (10 - size);
    if (filter_order > 4) {
        int code_size;
        int code_size_base = size - get_bits1(gb);
        for (i = 4; i < filter_order; i++) {
            if (!(i & 3)) code_size = code_size_base - get_bits(gb, 2);
            predictors[i] = get_sbits(gb, code_size) << (10 - size);
        }
    }
    filter_tmp[0] = predictors[0] << 6;
    for (i = 1; i < filter_order; i++) {
        int *p1 = &filter_tmp[0];
        int *p2 = &filter_tmp[i - 1];
        for (j = 0; j < (i + 1) / 2; j++) {
            int tmp = *p1 + (predictors[i] * *p2 + 256 >> 9);
            *p2 = *p2 + (predictors[i] * *p1 + 256 >> 9);
            *p1 = tmp;
            p1++;
            p2--;
        }
        filter_tmp[i] = predictors[i] << 6;
    }
    int a = 1 << (32 - (15 - filter_quant));
    int b = 1 << ((15 - filter_quant) - 1);
    for (i = 0, j = filter_order - 1; i < filter_order / 2; i++, j--) {
        filter[j] = a - ((filter_tmp[i] + b) >> (15 - filter_quant));
        filter[i] = a - ((filter_tmp[j] + b) >> (15 - filter_quant));
    }
}