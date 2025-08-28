static void rv40_parse_picture_size(GetBitContext *gb, int *w, int *h) {
    *w = get_dimension(gb, rv40_standard_widths);
    *h = get_dimension(gb, rv40_standard_heights);
}