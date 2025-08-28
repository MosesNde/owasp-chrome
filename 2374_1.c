static void decouple_info(COOKContext *q, COOKSubpacket *p, int *decouple_tab) {
    int i;
    int vlc = get_bits1(&q->gb);
    int start = cplband[p->js_subband_start];
    int end = cplband[p->subbands - 1];
    int length = end - start + 1;
    if (start > end) return;
    if (vlc) {
        for (i = 0; i < length; i++) {
            decouple_tab[start + i] = get_vlc2(NULL, p->channel_coupling.table, p->channel_coupling.bits, 2);
        }
    } else {
        for (i = 0; i < length; i++) {
            decouple_tab[start + i] = get_bits(NULL, p->js_vlc_bits);
        }
    }
}