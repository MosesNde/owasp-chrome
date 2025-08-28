static void decode_gains(AMRContext *p, const AMRNBSubframe *amr_subframe, const enum Mode mode, const int subframe, float *fixed_gain_factor) {
 if (mode == MODE_12k2 || mode == MODE_7k95) {
  p->pitch_gain[4] = qua_gain_pit[amr_subframe->p_gain] * (1.0 / 16384.0);
  *fixed_gain_factor = qua_gain_code[amr_subframe->fixed_gain] * (1.0 / 2048.0);
 } else {
  const uint16_t *gains;
  if (mode >= MODE_6k7) {
   gains = gains_high[amr_subframe->p_gain];
  } else if (mode >= MODE_5k15) {
   gains = gains_low[amr_subframe->p_gain];
  } else {
   gains = gains_MODE_4k75[(p->frame.subframe[subframe & 2].p_gain << 1) + (subframe & 1)];
  }
  p->pitch_gain[4] = gains[0] * (1.0 / 16384.0);
  *fixed_gain_factor = gains[1] * (1.0 / 4096.0);
 }
}