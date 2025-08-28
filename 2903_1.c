static void unpack_roq_qcell(uint8_t cb2[], roq_qcell *qcell, uint8_t u[4 * 4 * 3]) {
    int i, cp;
    static const int offsets[4] = {0, 2, 8, 10};
    for (cp = 0; cp < 3; cp++) {
        for (i = 0; i < 4; i++) {
            u[4 * 4 * cp + offsets[i]] = cb2[qcell->idx[i] * 2 * 2 * 3 + 4 * cp];
            u[4 * 4 * cp + offsets[i] + 1] = cb2[qcell->idx[i] * 2 * 2 * 3 + 4 * cp + 1];
            u[4 * 4 * cp + offsets[i] + 4] = cb2[qcell->idx[i] * 2 * 2 * 3 + 4 * cp + 2];
            u[4 * 4 * cp + offsets[i] + 5] = cb2[qcell->idx[i] * 2 * 2 * 3 + 4 * cp + 3];
        }
    }
}