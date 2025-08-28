static int get_value_cached(GetBitContext *gb, int vec_pos, uint8_t *vec, int vec_size, int component, int shift, int *prev) {
 if (vec_pos <= vec_size) return vec[vec_pos];
 if (!get_bits1(gb)) return prev[component];
 prev[component] = get_bits(gb, 8 - shift) << shift;
 return prev[component];
}