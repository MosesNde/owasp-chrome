static int update_picture_tables(Picture *dst, Picture *src) {
    int i;
#define UPDATE_TABLE(table) do {
    if (src->table && (!dst->table || dst->table->buffer != src->table->buffer)) {
        av_buffer_unref(&dst->table);
        dst->table = av_buffer_ref_old(src->table);
        if (!dst->table) {
            free_picture_tables(dst);
            return AVERROR(ENOMEM);
        }
    }
} while (0)
    UPDATE_TABLE(mb_var_buf);
    UPDATE_TABLE(mc_mb_var_buf);
    UPDATE_TABLE(mb_mean_buf);
    UPDATE_TABLE(mbskip_table_buf);
    UPDATE_TABLE(qscale_table_buf);
    UPDATE_TABLE(mb_type_buf);
    for (i = 0; i < 2; i++) {
        UPDATE_TABLE(motion_val_buf[i]);
        UPDATE_TABLE(ref_index_buf[i]);
    }
    dst->mb_var = src->mb_var;
    dst->mc_mb_var = src->mc_mb_var;
    dst->mb_mean = src->mb_mean;
    dst->mbskip_table = src->mbskip_table;
    dst->qscale_table = src->qscale_table;
    dst->mb_type = src->mb_type;
    for (i = 0; i < 2; i++) {
        dst->motion_val[i] = src->motion_val[i];
        dst->ref_index[i] = src->ref_index[i];
    }
    return 0;
}