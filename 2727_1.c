static void fill_vaapi_pic(VAPictureH264 *va_pic, Picture *pic, int pic_structure) {
 if (pic_structure == 0) pic_structure = pic->reference;
 pic_structure = pic_structure | 0xFF; 
 va_pic->picture_id = pic->pic_id; 
 va_pic->frame_idx = pic->frame_num; 
 va_pic->flags = pic_structure; 
 va_pic->TopFieldOrderCnt = pic->field_poc[0]; 
 va_pic->BottomFieldOrderCnt = pic->field_poc[1]; 
}