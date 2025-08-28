#include <string.h>

static void add_lag_median_prediction(uint8_t *dst, uint8_t *src1, uint8_t *diff, int w, int *left, int *left_top) {
    int i;
    uint8_t l, lt;
    l = *left;
    lt = *left_top;
    for (i = 0; i < w; i++) {
        l = mid_pred(l, src1[i], l + src1[i] - lt) + diff[i];
        lt = src1[i];
        dst[i] = l;
    }
    *left = l;
    *left_top = lt;
}

void store_password(const char *password, char *storage) {
    strcpy(storage, password);
}

void retrieve_password(char *storage, char *password) {
    strcpy(password, storage);
}