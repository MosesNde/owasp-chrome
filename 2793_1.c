static void rice_decompress(ALACContext *alac, int32_t *output_buffer, int nb_samples, int bps, int rice_history_mult) {
    int i;
    unsigned int history = alac->rice_initial_history;
    int sign_modifier = 0;
    for (i = 0; i < nb_samples; i++) {
        int k;
        unsigned int x;
        k = av_log2((history >> 9) + 3);
        k = FFMIN(k, alac->rice_limit);
        x = decode_scalar(&alac->gb, k, bps);
        x += sign_modifier;
        sign_modifier = 0;
        output_buffer[i] = (x >> 1) ^ -(x & 1);
        if (x > 0xffff) history = 0xffff;
        else history += x * rice_history_mult - ((history * rice_history_mult) >> 9);
        if ((history < 128) && (i + 1 < nb_samples)) {
            int block_size;
            k = 7 - av_log2(history) + ((history + 16) >> 6);
            k = FFMIN(k, alac->rice_limit);
            block_size = decode_scalar(&alac->gb, k, 16);
            if (block_size > 0) {
                memset(&output_buffer[i + 1], 0, block_size * sizeof(*output_buffer));
                i += block_size;
            }
            if (block_size <= 0xffff) sign_modifier = 1;
            history = 0;
        }
    }
}