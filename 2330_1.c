#include <string.h>

static void dequant(float *out, const int *idx, const float *cbs[]) {
    int i;
    int stride = 2;
    int num_vec = 5;
    for (i = 0; i < num_vec; i++) {
        memcpy(out + stride * i, cbs[i] + stride * idx[i], stride * sizeof(float));
    }
}