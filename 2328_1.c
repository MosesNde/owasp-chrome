static void decode_fixed_sparse(AMRFixed *fixed_sparse, const int16_t *pulses, SiprMode mode, int low_gain) {
    int i;
    switch (mode) {
        case MODE_6k5:
            for (i = 0; i < 3; i++) {
                fixed_sparse->x[i] = 3 * (pulses[i] & 0xf) + i;
                fixed_sparse->y[i] = pulses[i] & 0x10 ? -1 : 1;
            }
            fixed_sparse->n = 3;
            break;
        case MODE_8k5:
            for (i = 0; i < 3; i++) {
                fixed_sparse->x[2 * i] = 3 * ((pulses[i] >> 4) & 0xf) + i;
                fixed_sparse->x[2 * i + 1] = 3 * (pulses[i] & 0xf) + i;
                fixed_sparse->y[2 * i] = (pulses[i] & 0x100) ? -1 : 1;
                fixed_sparse->y[2 * i + 1] = (fixed_sparse->x[2 * i + 1] < fixed_sparse->x[2 * i]) ? -fixed_sparse->y[2 * i] : fixed_sparse->y[2 * i];
            }
            fixed_sparse->n = 6;
            break;
        case MODE_5k0:
        default:
            if (low_gain) {
                int offset = (pulses[0] & 0x200) ? 2 : 0;
                int val = pulses[0];
                for (i = 0; i < 3; i++) {
                    int index = (val & 0x7) * 6 + 4 - i * 2;
                    fixed_sparse->y[i] = (offset + index) & 0x3 ? -1 : 1;
                    fixed_sparse->x[i] = index;
                    val >>= 3;
                }
                fixed_sparse->n = 3;
            } else {
                int pulse_subset = (pulses[0] >> 8) & 1;
                fixed_sparse->x[0] = ((pulses[0] >> 4) & 15) * 3 + pulse_subset;
                fixed_sparse->x[1] = (pulses[0] & 15) * 3 + pulse_subset + 1;
                fixed_sparse->y[0] = pulses[0] & 0x200 ? -1 : 1;
                fixed_sparse->y[1] = -fixed_sparse->y[0];
                fixed_sparse->n = 2;
            }
            break;
    }
}