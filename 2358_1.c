static void decode_vectors(COOKContext *q, COOKSubpacket *p, int *category, int *quant_index_table, float *mlt_buffer) {
    int subband_coef_index[SUBBAND_SIZE];
    int subband_coef_sign[SUBBAND_SIZE];
    int band, j;
    int index = 0;
    for (band = 0; band < p->total_subbands; band++) {
        index = category[band];
        if (category[band] < 7) {
            if (unpack_SQVH(q, p, category[band], subband_coef_index, subband_coef_sign)) {
                index = 7;
                for (j = 0; j < p->total_subbands; j++) category[band + j] = 7;
            }
        }
        if (index >= 7) {
            memset(subband_coef_index, 0, sizeof(subband_coef_index));
            memset(subband_coef_sign, 0, sizeof(subband_coef_sign));
        }
        q->scalar_dequant(q, index, quant_index_table[band], subband_coef_index, subband_coef_sign, &mlt_buffer[band * SUBBAND_SIZE]);
    }
    if (p->total_subbands * SUBBAND_SIZE >= q->samples_per_channel) return;
}