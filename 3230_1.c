static void rv34_mc_1mv(RV34DecContext *r, const int block_type, const int xoff, const int yoff, int mv_off, const int width, const int height, int dir) {
    rv34_mc(r, block_type, xoff, yoff, mv_off, width, height, dir, r->rv30, 0, r->rdsp.put_pixels_tab, r->rdsp.put_chroma_pixels_tab);
}