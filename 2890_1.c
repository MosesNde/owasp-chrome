static void revert_inter_ch_decorr(WmallDecodeCtx *s, int tile_size) {
    if (s->num_channels != 2) return;
    if (s->is_channel_coded[0] || s->is_channel_coded[1]) {
        int icoef;
        for (icoef = 0; icoef < tile_size; icoef++) {
            s->channel_residues[0][icoef] -= s->channel_residues[1][icoef] >> 1;
            s->channel_residues[1][icoef] += s->channel_residues[0][icoef];
        }
    }
}