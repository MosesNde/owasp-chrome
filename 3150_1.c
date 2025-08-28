static void svq1_write_header(SVQ1Context *s, int frame_type, int user_role) {
 int i;
 put_bits(&s->pb, 22, 0x20);
 put_bits(&s->pb, 8, 0x00);
 put_bits(&s->pb, 2, frame_type - 1);
 if (frame_type == AV_PICTURE_TYPE_I) {
  put_bits(&s->pb, 5, 2);
  i = ff_match_2uint16(ff_svq1_frame_size_table, FF_ARRAY_ELEMS(ff_svq1_frame_size_table), s->frame_width, s->frame_height);
  put_bits(&s->pb, 3, i);
  if (i == 7) {
   put_bits(&s->pb, 12, s->frame_width);
   put_bits(&s->pb, 12, s->frame_height);
  }
 }
 put_bits(&s->pb, 2, 0);
}