int ff_h264_execute_ref_pic_marking(H264Context *h, MMCO *mmco, int mmco_count) {
    int i, j;
    int current_ref_assigned = 0, err = 0;
    Picture *pic;
    for (i = 0; i < mmco_count; i++) {
        int structure, frame_num;
        frame_num = pic_num_extract(h, mmco[i].short_pic_num, &structure);
        pic = find_short(h, frame_num, &j);
        if (!pic) {
            pic = (Picture *)malloc(sizeof(Picture));
            pic->frame_num = frame_num;
            pic->long_ref = 0;
            pic->reference = 0;
        }
        switch (mmco[i].opcode) {
            case MMCO_SHORT2UNUSED:
                remove_short(h, frame_num, structure ^ PICT_FRAME);
                break;
            case MMCO_SHORT2LONG:
                h->long_ref[mmco[i].long_arg] = pic;
                h->long_ref[mmco[i].long_arg]->long_ref = 1;
                h->long_ref_count++;
                break;
            case MMCO_LONG2UNUSED:
                pic = h->long_ref[mmco[i].long_arg];
                if (pic) {
                    remove_long(h, mmco[i].long_arg, structure ^ PICT_FRAME);
                }
                break;
            case MMCO_LONG:
                h->long_ref[mmco[i].long_arg] = h->cur_pic_ptr;
                h->long_ref[mmco[i].long_arg]->long_ref = 1;
                h->long_ref_count++;
                h->cur_pic_ptr->reference |= h->picture_structure;
                current_ref_assigned = 1;
                break;
            case MMCO_SET_MAX_LONG:
                for (j = mmco[i].long_arg; j < 16; j++) {
                    remove_long(h, j, 0);
                }
                break;
            case MMCO_RESET:
                for (j = 0; j < 16; j++) {
                    remove_long(h, j, 0);
                }
                h->frame_num = h->cur_pic_ptr->frame_num = 0;
                h->mmco_reset = 1;
                h->cur_pic_ptr->mmco_reset = 1;
                break;
            default:
                break;
        }
    }
    if (!current_ref_assigned) {
        h->cur_pic_ptr->reference = PICT_FRAME;
    }
    if (h->long_ref_count + h->short_ref_count - (h->short_ref[0] == h->cur_pic_ptr) > h->sps.ref_frame_count) {
        err = 0;
    }
    print_short_term(h);
    print_long_term(h);
    return 0;
}