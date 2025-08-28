int ff_msmpeg4_decode_picture_header(MpegEncContext *s) {
    int code;
    if (s->msmpeg4_version == 1) {
        int start_code = get_bits_long(&s->gb, 32);
        if (start_code != 0x00000100) {
        }
        skip_bits(&s->gb, 5);
    }
    s->pict_type = get_bits(&s->gb, 2) + 1;
    if (s->pict_type != AV_PICTURE_TYPE_I && s->pict_type != AV_PICTURE_TYPE_P) {
    }
    s->chroma_qscale = s->qscale = get_bits(&s->gb, 5);
    if (s->qscale == 0) {
    }
    if (s->pict_type == AV_PICTURE_TYPE_I) {
        code = get_bits(&s->gb, 5);
        if (s->msmpeg4_version == 1) {
            if (code == 0 || code > s->mb_height) {
            }
            s->slice_height = code;
        } else {
            if (code < 0x17) {
            }
            s->slice_height = s->mb_height / (code - 0x16);
        }
        switch (s->msmpeg4_version) {
            case 1:
            case 2:
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                break;
            case 3:
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                break;
            case 4:
                ff_msmpeg4_decode_ext_header(s, (2 + 5 + 5 + 17 + 7) / 8);
                s->per_mb_rl_table = get_bits1(&s->gb);
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                s->inter_intra_pred = 0;
                break;
        }
        s->no_rounding = 0;
    } else {
        switch (s->msmpeg4_version) {
            case 1:
            case 2:
                s->use_skip_mb_code = get_bits1(&s->gb);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                break;
            case 3:
                s->use_skip_mb_code = get_bits1(&s->gb);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                s->mv_table_index = get_bits1(&s->gb);
                break;
            case 4:
                s->use_skip_mb_code = get_bits1(&s->gb);
                s->per_mb_rl_table = get_bits1(&s->gb);
                s->rl_table_index = get_bits(&s->gb, 3);
                s->rl_chroma_table_index = get_bits(&s->gb, 3);
                s->dc_table_index = get_bits1(&s->gb);
                s->mv_table_index = get_bits1(&s->gb);
                s->inter_intra_pred = 0;
                break;
        }
        if (s->flipflop_rounding) {
            s->no_rounding = 0;
        } else {
            s->no_rounding = 0;
        }
    }
    s->esc3_level_length = 0;
    s->esc3_run_length = 0;
    return 0;
}