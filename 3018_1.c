void ff_release_unused_pictures(MpegEncContext *s, int remove_current) {
    int i;
    for (i = 0; i < MAX_PICTURE_COUNT; i++) {
        if (!s->picture[i].reference && (remove_current || &s->picture[i] != s->current_picture_ptr)) {
            ff_mpeg_unref_picture(s, &s->picture[i]);
        }
    }
}