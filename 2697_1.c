static void permutate_in_line(int16_t *tab, int num_vect, int num_blocks, int block_size, const uint8_t line_len[2], int length_div, enum FrameType ftype, const uint8_t *data, size_t data_len) {
 int i, j;
 for (i = 0; i < line_len[0]; i++) {
  int shift;
  if (num_blocks == 1 || (ftype == FT_LONG && num_vect % num_blocks) || (ftype != FT_LONG && num_vect & 1) || i == line_len[1]) {
   shift = 0;
  } else if (ftype == FT_LONG) {
   shift = i;
  } else shift = i * i;
  for (j = 0; j < num_vect && (j + num_vect * i < block_size * num_blocks); j++) tab[i * num_vect + j] = i * num_vect + (j + shift) % num_vect;
 }
}