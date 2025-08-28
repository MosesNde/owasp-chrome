void ff_vdpau_vc1_decode_picture(MpegEncContext *s, const uint8_t *buf, int buf_size) {
    VC1Context *v = s->avctx->priv_data;
    struct vdpau_render_state *render, *last, *next;
    render = (struct vdpau_render_state *)s->current_picture.f.data[0];
    render->info.vc1.frame_coding_mode = v->fcm;
    render->info.vc1.postprocflag = v->postprocflag;
    render->info.vc1.pulldown = v->broadcast;
    render->info.vc1.interlace = v->interlace;
    render->info.vc1.tfcntrflag = v->tfcntrflag;
    render->info.vc1.finterpflag = v->finterpflag;
    render->info.vc1.psf = v->psf;
    render->info.vc1.dquant = v->dquant;
    render->info.vc1.panscan_flag = v->panscanflag;
    render->info.vc1.refdist_flag = v->refdist_flag;
    render->info.vc1.quantizer = v->quantizer_mode;
    render->info.vc1.extended_mv = v->extended_mv;
    render->info.vc1.extended_dmv = v->extended_dmv;
    render->info.vc1.overlap = v->overlap;
    render->info.vc1.vstransform = v->vstransform;
    render->info.vc1.loopfilter = v->s.loop_filter;
    render->info.vc1.fastuvmc = v->fastuvmc;
    render->info.vc1.range_mapy_flag = v->range_mapy_flag;
    render->info.vc1.range_mapy = v->range_mapy;
    render->info.vc1.range_mapuv_flag = v->range_mapuv_flag;
    render->info.vc1.range_mapuv = v->range_mapuv;
    render->info.vc1.multires = v->multires;
    render->info.vc1.syncmarker = v->s.resync_marker;
    render->info.vc1.rangered = v->rangered | (v->rangeredfrm << 1);
    render->info.vc1.maxbframes = v->s.max_b_frames;
    render->info.vc1.deblockEnable = v->postprocflag & 1;
    render->info.vc1.pquant = v->pq;
    render->info.vc1.forward_reference = VDP_INVALID_HANDLE;
    render->info.vc1.backward_reference = VDP_INVALID_HANDLE;
    if (v->bi_type)
        render->info.vc1.picture_type = 4;
    else
        render->info.vc1.picture_type = s->pict_type - 1 + s->pict_type / 3;
    switch (s->pict_type) {
    case AV_PICTURE_TYPE_B:
        next = (struct vdpau_render_state *)s->next_picture.f.data[0];
        render->info.vc1.backward_reference = next->surface;
    case AV_PICTURE_TYPE_P:
        last = (struct vdpau_render_state *)s->last_picture.f.data[0];
        render->info.vc1.forward_reference = last->surface;
    }
    ff_vdpau_add_data_chunk(s->current_picture_ptr->f.data[0], buf, buf_size);
    render->info.vc1.slice_count = 1;
    ff_mpeg_draw_horiz_band(s, 0, s->avctx->height);
    render->bitstream_buffers_used = 0;
}