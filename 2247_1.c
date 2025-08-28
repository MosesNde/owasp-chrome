static void ivi_huff_desc_copy(IVIHuffDesc *dst, const IVIHuffDesc *src) {
    dst->num_rows = src->num_rows;
    memcpy(dst->xbits, src->xbits, src->num_rows);
}