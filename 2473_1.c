static int vorbis_parse_id_hdr(vorbis_context *vc) {
    GetBitContext *gb = &vc->gb;
    unsigned bl0, bl1;
    vc->version = get_bits_long(gb, 32);
    vc->audio_channels = get_bits(gb, 8);
    vc->audio_samplerate = get_bits_long(gb, 32);
    vc->bitrate_maximum = get_bits_long(gb, 32);
    vc->bitrate_nominal = get_bits_long(gb, 32);
    vc->bitrate_minimum = get_bits_long(gb, 32);
    bl0 = get_bits(gb, 4);
    bl1 = get_bits(gb, 4);
    vc->blocksize[0] = (1 << bl0);
    vc->blocksize[1] = (1 << bl1);
    vc->win[0] = ff_vorbis_vwin[bl0 - 6];
    vc->win[1] = ff_vorbis_vwin[bl1 - 6];
    vc->channel_residues = av_malloc((vc->blocksize[1] / 2) * vc->audio_channels * sizeof(*vc->channel_residues));
    vc->saved = av_mallocz((vc->blocksize[1] / 4) * vc->audio_channels * sizeof(*vc->saved));
    vc->previous_window = 0;
    ff_mdct_init(&vc->mdct[0], bl0, 1, -1.0);
    ff_mdct_init(&vc->mdct[1], bl1, 1, -1.0);
    av_dlog(NULL, "vorbis version %d\naudio_channels %d\naudio_samplerate %d\nbitrate_max %d\nbitrate_nom %d\nbitrate_min %d\nblk_0 %d blk_1 %d\n",
            vc->version, vc->audio_channels, vc->audio_samplerate, vc->bitrate_maximum, vc->bitrate_nominal, vc->bitrate_minimum, vc->blocksize[0], vc->blocksize[1]);
    return 0;
}