static int mpeg4_decode_profile_level(MpegEncContext *s, GetBitContext *gb) {
    int profile_and_level_indication;
    profile_and_level_indication = get_bits(gb, 8);
    s->avctx->profile = profile_and_level_indication;
    s->avctx->level = profile_and_level_indication;
    return 0;
}