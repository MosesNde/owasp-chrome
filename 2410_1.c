static void libopenjpeg_copy_to_packed8(AVFrame *picture, opj_image_t *image, int user_role) {
 uint8_t *img_ptr;
 int index, x, y, c;
 for (y = 0; y < picture->height; y++) {
  index = y * picture->width;
  img_ptr = picture->data[0] + y * picture->linesize[0];
  for (x = 0; x < picture->width; x++, index++) {
   for (c = 0; c < image->numcomps; c++) {
    *img_ptr++ = image->comps[c].data[index];
   }
  }
 }
}