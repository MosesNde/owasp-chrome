static void read_vec_pos(GetBitContext *gb, int *vec_pos, int *sel_flag, int *sel_len, int *prev) {
 int i, y_flag = 0;
 for (i = 2; i >= 0; i--) {
 if (!sel_flag[i]) {
 vec_pos[i] = 0;
 continue;
 }
 if ((!i && !y_flag) || get_bits1(gb)) {
 if (sel_len[i] > 0) {
 int pval = prev[i];
 vec_pos[i] = get_bits_old(gb, sel_len[i]);
 if (vec_pos[i] >= pval) vec_pos[i]++;
 } else {
 vec_pos[i] = !prev[i];
 }
 y_flag = 1;
 } else {
 vec_pos[i] = prev[i];
 }
 }
}