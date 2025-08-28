static inline float get_cos(int idx, int part, const float *cos_tab, int size) {
 return part ? -cos_tab[size - idx - 1] : cos_tab[idx];
}