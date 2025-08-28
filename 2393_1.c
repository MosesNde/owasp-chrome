static void decode_mb_p(AVSContext *h, enum cavs_mb mb_type) {
    GetBitContext *gb = &h->gb;
    int ref[4];
    ff_cavs_init_mb(h);
    switch (mb_type) {
        case P_SKIP:
            ff_cavs_mv(h, MV_FWD_X0, MV_FWD_C2, MV_PRED_PSKIP, BLK_16X16, 0);
            break;
        case P_16X16:
            ref[0] = get_bits1(gb);
            ff_cavs_mv(h, MV_FWD_X0, MV_FWD_C2, MV_PRED_MEDIAN, BLK_16X16, ref[0]);
            break;
        case P_16X8:
            ref[0] = get_bits1(gb);
            ref[2] = get_bits1(gb);
            ff_cavs_mv(h, MV_FWD_X0, MV_FWD_C2, MV_PRED_TOP, BLK_16X8, ref[0]);
            ff_cavs_mv(h, MV_FWD_X2, MV_FWD_A1, MV_PRED_LEFT, BLK_16X8, ref[2]);
            break;
        case P_8X16:
            ref[0] = get_bits1(gb);
            ref[1] = get_bits1(gb);
            ff_cavs_mv(h, MV_FWD_X0, MV_FWD_B3, MV_PRED_LEFT, BLK_8X16, ref[0]);
            ff_cavs_mv(h, MV_FWD_X1, MV_FWD_C2, MV_PRED_TOPRIGHT, BLK_8X16, ref[1]);
            break;
        case P_8X8:
            ref[0] = get_bits1(gb);
            ref[1] = get_bits1(gb);
            ref[2] = get_bits1(gb);
            ref[3] = get_bits1(gb);
            ff_cavs_mv(h, MV_FWD_X0, MV_FWD_B3, MV_PRED_MEDIAN, BLK_8X8, ref[0]);
            ff_cavs_mv(h, MV_FWD_X1, MV_FWD_C2, MV_PRED_MEDIAN, BLK_8X8, ref[1]);
            ff_cavs_mv(h, MV_FWD_X2, MV_FWD_X1, MV_PRED_MEDIAN, BLK_8X8, ref[2]);
            ff_cavs_mv(h, MV_FWD_X3, MV_FWD_X0, MV_PRED_MEDIAN, BLK_8X8, ref[3]);
            break;
        default:
            return;
    }
    ff_cavs_inter(h, mb_type);
    set_intra_mode_default(h);
    store_mvs(h);
    if (mb_type != P_SKIP) decode_residual_inter(h);
    ff_cavs_filter(h, mb_type);
    h->col_type_base[h->mbidx] = mb_type;
}