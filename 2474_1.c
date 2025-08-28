static int vorbis_floor0_decode(vorbis_context *vc, vorbis_floor_data *vfu, float *vec) {
    vorbis_floor0 *vf = &vfu->t0;
    float *lsp = vf->lsp;
    unsigned amplitude, book_idx;
    unsigned blockflag = vc->modes[vc->mode_number].blockflag;
    amplitude = get_bits(&vc->gb, vf->amplitude_bits);
    if (amplitude > 0) {
        float last = 0;
        unsigned idx, lsp_len = 0;
        vorbis_codebook codebook;
        book_idx = get_bits(&vc->gb, ilog(vf->num_books));
        codebook = vc->codebooks[vf->book_list[book_idx]];
        while (lsp_len < vf->order) {
            int vec_off;
            vec_off = get_vlc2(&vc->gb, codebook.vlc.table, codebook.nb_bits, codebook.maxdepth) * codebook.dimensions;
            for (idx = 0; idx < codebook.dimensions; ++idx) lsp[lsp_len + idx] = codebook.codevectors[vec_off + idx] + last;
            last = lsp[lsp_len + idx - 1];
            lsp_len += codebook.dimensions;
        }
        {
            int i;
            int order = vf->order;
            float wstep = M_PI / vf->bark_map_size;
            for (i = 0; i < order; i++) lsp[i] = 2.0f * cos(lsp[i]);
            i = 0;
            while (i < vf->map_size[blockflag]) {
                int j, iter_cond = vf->map[blockflag][i];
                float p = 0.5f;
                float q = 0.5f;
                float two_cos_w = 2.0f * cos(wstep * iter_cond);
                for (j = 0; j + 1 < order; j += 2) {
                    q *= lsp[j] - two_cos_w;
                    p *= lsp[j + 1] - two_cos_w;
                }
                if (j == order) {
                    p *= p * (2.0f - two_cos_w);
                    q *= q * (2.0f + two_cos_w);
                } else {
                    q *= two_cos_w - lsp[j];
                    p *= p * (4.f - two_cos_w * two_cos_w);
                    q *= q;
                }
                q = exp((((amplitude * vf->amplitude_offset) / (((1 << vf->amplitude_bits) - 1) * sqrt(p + q))) - vf->amplitude_offset) * .11512925f);
                do {
                    vec[i] = q;
                    ++i;
                } while (vf->map[blockflag][i] == iter_cond);
            }
        }
    } else {
        return 1;
    }
    return 0;
}