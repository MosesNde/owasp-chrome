static int make_tables_writable(Picture *pic) {
    int ret, i;
    #define MAKE_WRITABLE(table) do {
        if (pic->table && (ret = av_buffer_make_writable(&pic->table)) < 0) {
            return ret;
        }
    } while (0)
    MAKE_WRITABLE(mb_var_buf);
    MAKE_WRITABLE(mc_mb_var_buf);
    MAKE_WRITABLE(mb_mean_buf);
    MAKE_WRITABLE(mbskip_table_buf);
    MAKE_WRITABLE(qscale_table_buf);
    MAKE_WRITABLE(mb_type_buf);
    for (i = 0; i < 2; i++) {
        MAKE_WRITABLE(motion_val_buf[i]);
        MAKE_WRITABLE(ref_index_buf[i]);
    }
    return 0;
}