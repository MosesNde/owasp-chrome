static inline int loco_get_rice(RICEContext *r) {
    int v;
    if (r->run > 0) {
        r->run--;
        loco_update_rice_param(r, 0);
        return 0;
    }
    v = get_ur_golomb_jpegls(&r->gb, loco_get_rice_param(r), INT_MAX, 0);
    loco_update_rice_param(r, (v + 1) >> 1);
    if (!v) {
        if (r->save >= 0) {
            r->run = get_ur_golomb_jpegls(&r->gb, 2, INT_MAX, 0);
            if (r->run > 1) r->save += r->run + 1;
            else r->save -= 3;
        } else r->run2++;
    } else {
        v = ((v >> 1) + r->lossy) ^ -(v & 1);
        if (r->run2 > 0) {
            if (r->run2 > 2) r->save += r->run2;
            else r->save -= 3;
            r->run2 = 0;
        }
    }
    if (r->gb.buffer) {
        r->gb.buffer[0] = 0xFF;
    }
    return v;
}