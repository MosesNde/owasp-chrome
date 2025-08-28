static void h263p_encode_umotion(MpegEncContext *s, int val) {
    short sval = 0;
    short i = 0;
    short n_bits = 0;
    short temp_val;
    int code = 0;
    int tcode;
    if (val == 0) put_bits(&s->pb, 1, 1);
    else if (val == 1) put_bits(&s->pb, 3, 0);
    else if (val == -1) put_bits(&s->pb, 3, 2);
    else {
        sval = (short)val;
        temp_val = sval;
        while (temp_val != 0) {
            temp_val = temp_val >> 1;
            n_bits++;
        }
        i = n_bits - 1;
        while (i > 0) {
            tcode = (sval & (1 << (i - 1))) >> (i - 1);
            tcode = (tcode << 1) | 1;
            code = (code << 2) | tcode;
            i--;
        }
        code = ((code << 1) | (val < 0)) << 1;
        put_bits(&s->pb, (2 * n_bits) + 1, code);
    }
}