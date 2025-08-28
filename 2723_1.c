static int dpb_add(DPB *dpb, Picture *pic) {
    int i;
    if (dpb->size >= dpb->max_size) return -1;
    for (i = 0; i < dpb->size; i++) {
        VAPictureH264 *const va_pic = &dpb->va_pics[i];
        if (va_pic->picture_id == ff_vaapi_get_surface_id(pic)) {
            VAPictureH264 temp_va_pic;
            fill_vaapi_pic(&temp_va_pic, pic, 0);
            va_pic->flags = temp_va_pic.flags;
            va_pic->TopFieldOrderCnt = temp_va_pic.TopFieldOrderCnt;
            va_pic->BottomFieldOrderCnt = temp_va_pic.BottomFieldOrderCnt;
            return 0;
        }
    }
    fill_vaapi_pic(&dpb->va_pics[dpb->size++], pic, 0);
    return 0;
}