void ff_h263_encode_gob_header(MpegEncContext *s, int mb_line) {
    put_bits(&s->pb, 17, 1);
    if (s->h263_slice_structured) {
        put_bits(&s->pb, 1, 1);
        ff_h263_encode_mba(s);
        if (s->mb_num > 1583) put_bits(&s->pb, 1, 1);
        put_bits(&s->pb, 5, s->qscale);
        put_bits(&s->pb, 1, 1);
        put_bits(&s->pb, 2, s->pict_type == AV_PICTURE_TYPE_I);
    } else {
        int gob_number = mb_line / s->gob_index;
        put_bits(&s->pb, 5, gob_number);
        put_bits(&s->pb, 2, s->pict_type == AV_PICTURE_TYPE_I);
        put_bits(&s->pb, 5, s->qscale);
    }
}