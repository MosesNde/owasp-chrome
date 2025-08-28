int ff_msmpeg4_pred_dc(MpegEncContext *s, int n, int16_t **dc_val_ptr, int *dir_ptr) {
    int a, b, c, wrap, pred, scale;
    int16_t *dc_val;
    if (n < 4) {
        scale = s->y_dc_scale;
    } else {
        scale = s->c_dc_scale;
    }
    wrap = s->block_wrap[n];
    dc_val = s->dc_val[0] + s->block_index[n];
    a = dc_val[-1];
    b = dc_val[-1 - wrap];
    c = dc_val[-wrap];
    if (s->first_slice_line && (n & 2) == 0 && s->msmpeg4_version < 4) {
        b = c = 1024;
    }
    if (scale == 8) {
        a = (a + (8 >> 1)) / 8;
        b = (b + (8 >> 1)) / 8;
        c = (c + (8 >> 1)) / 8;
    } else {
        a = (a + (scale >> 1)) / scale;
        b = (b + (scale >> 1)) / scale;
        c = (c + (scale >> 1)) / scale;
    }
    if (s->msmpeg4_version > 3) {
        if (s->inter_intra_pred) {
            uint8_t *dest;
            int wrap;
            if (n == 1) {
                pred = a;
                *dir_ptr = 0;
            } else if (n == 2) {
                pred = c;
                *dir_ptr = 1;
            } else if (n == 3) {
                if (abs(a - b) < abs(b - c)) {
                    pred = c;
                    *dir_ptr = 1;
                } else {
                    pred = a;
                    *dir_ptr = 0;
                }
            } else {
                if (n < 4) {
                    wrap = s->linesize;
                    dest = s->current_picture.f.data[0] + (((n >> 1) + 2 * s->mb_y) * 8 * wrap) + ((n & 1) + 2 * s->mb_x) * 8;
                } else {
                    wrap = s->uvlinesize;
                    dest = s->current_picture.f.data[n - 3] + (s->mb_y * 8 * wrap) + s->mb_x * 8;
                }
                if (s->mb_x == 0) a = (1024 + (scale >> 1)) / scale;
                else a = get_dc(dest - 8, wrap, scale * 8);
                if (s->mb_y == 0) c = (1024 + (scale >> 1)) / scale;
                else c = get_dc(dest - 8 * wrap, wrap, scale * 8);
                if (s->h263_aic_dir == 0) {
                    pred = a;
                    *dir_ptr = 0;
                } else if (s->h263_aic_dir == 1) {
                    if (n == 0) {
                        pred = c;
                        *dir_ptr = 1;
                    } else {
                        pred = a;
                        *dir_ptr = 0;
                    }
                } else if (s->h263_aic_dir == 2) {
                    if (n == 0) {
                        pred = a;
                        *dir_ptr = 0;
                    } else {
                        pred = c;
                        *dir_ptr = 1;
                    }
                } else {
                    pred = c;
                    *dir_ptr = 1;
                }
            }
        } else {
            if (abs(a - b) < abs(b - c)) {
                pred = c;
                *dir_ptr = 1;
            } else {
                pred = a;
                *dir_ptr = 0;
            }
        }
    } else {
        if (abs(a - b) <= abs(b - c)) {
            pred = c;
            *dir_ptr = 1;
        } else {
            pred = a;
            *dir_ptr = 0;
        }
    }
    *dc_val_ptr = &dc_val[0];
    return pred;
}