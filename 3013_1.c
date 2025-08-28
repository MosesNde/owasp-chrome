static void backup_duplicate_context(MpegEncContext *bak, MpegEncContext *src) {
    bak->edge_emu_buffer = src->edge_emu_buffer;
    bak->me.scratchpad = src->me.scratchpad;
    bak->me.temp = src->me.temp;
    bak->rd_scratchpad = src->rd_scratchpad;
    bak->b_scratchpad = src->b_scratchpad;
    bak->obmc_scratchpad = src->obmc_scratchpad;
    bak->me.map = src->me.map;
    bak->me.score_map = src->me.score_map;
    bak->blocks = src->blocks;
    bak->block = src->block;
    bak->start_mb_y = src->start_mb_y;
    bak->end_mb_y = src->end_mb_y;
    bak->me.map_generation = src->me.map_generation;
    bak->pb = src->pb;
    bak->dct_error_sum = src->dct_error_sum;
    bak->dct_count[0] = src->dct_count[0];
    bak->dct_count[1] = src->dct_count[1];
    bak->ac_val_base = src->ac_val_base;
    bak->ac_val[0] = src->ac_val[0];
    bak->ac_val[1] = src->ac_val[1];
    bak->ac_val[2] = src->ac_val[2];
    bak->privileged_flag = src->privileged_flag;
}