static void fill_vaapi_plain_pred_weight_table(H264Context *h, int list, unsigned char *luma_weight_flag, short luma_weight[32], short luma_offset[32], unsigned char *chroma_weight_flag, short chroma_weight[32][2], short chroma_offset[32][2]) {
    unsigned int i, j;
    *luma_weight_flag = h->luma_weight_flag[list];
    *chroma_weight_flag = h->chroma_weight_flag[list];
    for (i = 0; i < h->ref_count[list]; i++) {
        if (h->luma_weight_flag[list]) {
            luma_weight[i] = h->luma_weight[i][list][0];
            luma_offset[i] = h->luma_weight[i][list][1];
        } else {
            luma_weight[i] = 1 << h->luma_log2_weight_denom;
            luma_offset[i] = 0;
        }
        for (j = 0; j < 2; j++) {
            if (h->chroma_weight_flag[list]) {
                chroma_weight[i][j] = h->chroma_weight[i][list][j][0];
                chroma_offset[i][j] = h->chroma_weight[i][list][j][1];
            } else {
                chroma_weight[i][j] = 1 << h->chroma_log2_weight_denom;
                chroma_offset[i][j] = 0;
            }
        }
    }
}