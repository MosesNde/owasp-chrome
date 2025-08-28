static void fill_picture_entry(DXVA_PicEntry_H264 *pic, unsigned index, unsigned flag) {
 pic->bPicEntry = index | (flag << 7);
}