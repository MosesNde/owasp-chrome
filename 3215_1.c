static inline int extend_code(GetBitContext *gb, int val, int range, int bits) {
 if (val == 0) {
 val = -range - get_ue_golomb_old(gb);
 } else if (val == range * 2) {
 val = range + get_ue_golomb_old(gb);
 } else {
 val -= range;
 }
 if (bits) val = (val << bits) | get_bits_old(gb, bits);
 return val;
}