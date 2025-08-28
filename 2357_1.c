#define FF_ARRAY_ELEMS(x) (sizeof(x) / sizeof((x)[0]))

static const int dither_tab[] = {0,1,2,3,4,5,6,7,8,9};

typedef struct {
    int num_vectors;
} COOKContext;

static inline void expand_category(COOKContext *q, int *category, int *category_index) {
    int i;
    for (i = 0; i < q->num_vectors; i++) {
        int idx = category_index[i];
        if (++category[idx] >= FF_ARRAY_ELEMS(dither_tab)) --category[idx];
    }
}