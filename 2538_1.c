typedef struct {
    int reference;
    int pic_id;
} Picture;

void COPY_PICTURE(Picture* dest, Picture* src) {
    *dest = *src;
}

void pic_as_field(Picture* pic, int parity) {
    pic->pic_id += parity;
}

static int split_field_copy(Picture* dest, Picture* src, int parity, int id_add) {
    int match = !!(src->reference & parity);
    if (match) {
        COPY_PICTURE(dest, src);
        if (parity != 0) {
            pic_as_field(dest, parity);
            dest->pic_id *= 2;
            dest->pic_id += id_add;
        }
    }
    return match;
}