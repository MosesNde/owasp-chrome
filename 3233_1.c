static inline void ZERO8x2(void *dst, int stride) {
    fill_rectangle(dst, 1, 2, stride, 0, 4);
    fill_rectangle(((uint8_t *)(dst)) + 8, 1, 2, stride, 0, 4);
}