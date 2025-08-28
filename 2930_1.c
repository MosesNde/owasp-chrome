static inline void frame_block_to_cell(uint8_t *block, uint8_t *const *data, int top, int left, const int *stride) {
 int i, j, u = 0, v = 0;
 for (i = 0; i < 2; i++) for (j = 0; j < 2; j++) {
 int x = (top + i) * stride[0] + left + j;
 *block++ = data[0][x];
 x = (top + i) * stride[1] + left + j;
 u += data[1][x];
 v += data[2][x];
 }
 *block++ = (u + 2) / 4;
 *block++ = (v + 2) / 4;
}