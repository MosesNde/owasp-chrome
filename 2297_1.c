static void rematrix_channels(MLPDecodeContext *m, unsigned int substr) {
    SubStream *s = &m->substream[substr];
    unsigned int mat, src_ch, i;
    unsigned int maxchan;
    maxchan = s->max_matrix_channel;
    if (!s->noise_type) {
        generate_2_noise_channels(m, substr);
        maxchan += 2;
    } else {
        fill_noise_buffer(m, substr);
    }
    for (mat = 0; mat < s->num_primitive_matrices; mat++) {
        int matrix_noise_shift = s->matrix_noise_shift[mat];
        unsigned int dest_ch = s->matrix_out_ch[mat];
        int32_t mask = MSB_MASK(s->quant_step_size[dest_ch]);
        int32_t *coeffs = s->matrix_coeff[mat];
        int index = s->num_primitive_matrices - mat;
        int index2 = 2 * index + 1;
        for (i = 0; i < s->blockpos; i++) {
            int32_t bypassed_lsb = m->bypassed_lsbs[i][mat];
            int32_t *samples = m->sample_buffer[i];
            int64_t accum = 0;
            for (src_ch = 0; src_ch <= maxchan; src_ch++) {
                accum += (int64_t)samples[src_ch] * coeffs[src_ch];
            }
            if (matrix_noise_shift) {
                accum += m->noise_buffer[index] << (matrix_noise_shift + 7);
                index += index2;
            }
            samples[dest_ch] = ((accum >> 14) & mask) + bypassed_lsb;
        }
    }
}