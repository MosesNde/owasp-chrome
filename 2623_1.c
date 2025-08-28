static void decode_isf_indices_46b(uint16_t *ind, float *isf_q) {
 int i;
 for (i = 0; i < 9; i++) isf_q[i] = dico1_isf[ind[0]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 7; i++) isf_q[i + 9] = dico2_isf[ind[1]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 3; i++) isf_q[i] += dico21_isf[ind[2]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 3; i++) isf_q[i + 3] += dico22_isf[ind[3]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 3; i++) isf_q[i + 6] += dico23_isf[ind[4]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 3; i++) isf_q[i + 9] += dico24_isf[ind[5]][i] * (1.0f / (1 << 15));
 for (i = 0; i < 4; i++) isf_q[i + 12] += dico25_isf[ind[6]][i] * (1.0f / (1 << 15));
}