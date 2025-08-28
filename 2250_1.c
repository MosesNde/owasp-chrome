int ff_ivi_dec_huff_desc(GetBitContext *gb, int desc_coded, int which_tab, IVIHuffTab *huff_tab, AVCodecContext *avctx) {
    int i, result;
    IVIHuffDesc new_huff;
    if (!desc_coded) {
        huff_tab->tab = (which_tab) ? &ivi_blk_vlc_tabs[7] : &ivi_mb_vlc_tabs[7];
    } else {
        huff_tab->tab_sel = get_bits(gb, 3);
        if (huff_tab->tab_sel == 7) {
            new_huff.num_rows = get_bits(gb, 4);
            if (!new_huff.num_rows) {
                av_log(avctx, AV_LOG_ERROR, "Empty custom Huffman table!\n");
                return AVERROR_INVALIDDATA;
            }
            for (i = 0; i < new_huff.num_rows; i++)
                new_huff.xbits[i] = get_bits(gb, 4);
            if (ivi_huff_desc_cmp(&new_huff, &huff_tab->cust_desc)) {
                ivi_huff_desc_copy(&huff_tab->cust_desc, &new_huff);
                if (huff_tab->cust_tab.table)
                    ff_free_vlc(&huff_tab->cust_tab);
                result = ivi_create_huff_from_desc(&huff_tab->cust_desc, &huff_tab->cust_tab, 0);
                if (result) {
                    huff_tab->cust_desc.num_rows = 0;
                    av_log(avctx, AV_LOG_ERROR, "Error while initializing custom vlc table!\n");
                    return result;
                }
            }
            huff_tab->tab = &huff_tab->cust_tab;
        } else {
            huff_tab->tab = (which_tab) ? &ivi_blk_vlc_tabs[huff_tab->tab_sel] : &ivi_mb_vlc_tabs[huff_tab->tab_sel];
        }
    }
    return 0;
}