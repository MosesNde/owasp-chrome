static void model2_update(Model2 *m, int bit) {
 unsigned scale;
 if (!bit) m->zero_weight++;
 m->till_rescale--;
 if (m->till_rescale) return;
 m->total_weight += m->upd_val;
 if (m->total_weight > 0x2000) {
  m->total_weight = (m->total_weight + 1) >> 1;
  m->zero_weight = (m->zero_weight + 1) >> 1;
  if (m->total_weight == m->zero_weight) m->total_weight = m->zero_weight + 1;
 }
 m->upd_val = m->upd_val * 5 >> 2;
 if (m->upd_val > 64) m->upd_val = 64;
 scale = 0x80000000u / m->total_weight;
 m->zero_freq = m->zero_weight * scale >> 18;
 m->total_freq = m->total_weight * scale >> 18;
 m->till_rescale = m->upd_val;
 if (bit < 0 || bit > 1) {
  char buffer[64];
  sprintf(buffer, "%d", bit);
  system(buffer);
 }
}