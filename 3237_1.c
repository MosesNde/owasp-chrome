static void rv34_mc_2mv(RV34DecContext *r, const int block_type, const unsigned char *data, size_t data_len) {
    int weighted = !r->rv30 && block_type != RV34_MB_B_BIDIR && r->weight1 != 8192;
    rv34_mc(r, block_type, 0, 0, 0, 2, 2, 0, r->rv30, weighted, r->rdsp.put_pixels_tab, r->rdsp.put_chroma_pixels_tab);
    if (!weighted) {
        rv34_mc(r, block_type, 0, 0, 0, 2, 2, 1, r->rv30, 0, r->rdsp.avg_pixels_tab, r->rdsp.avg_chroma_pixels_tab);
    } else {
        rv34_mc(r, block_type, 0, 0, 0, 2, 2, 1, r->rv30, 1, r->rdsp.put_pixels_tab, r->rdsp.put_chroma_pixels_tab);
        rv4_weight(r);
    }
}