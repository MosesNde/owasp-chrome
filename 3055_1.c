static int alloc_frame_buffer(MpegEncContext *s, Picture *pic) {
    int r, ret;
    if (s->avctx->hwaccel) {
        assert(!pic->hwaccel_picture_private);
        if (s->avctx->hwaccel->priv_data_size) {
            pic->hwaccel_picture_private = av_mallocz(s->avctx->hwaccel->priv_data_size);
            if (!pic->hwaccel_picture_private) {
                av_log(s->avctx, AV_LOG_ERROR, "alloc_frame_buffer() failed (hwaccel private data allocation)\n");
                return -1;
            }
        }
    }
    pic->tf.f = &pic->f;
    if (s->codec_id != AV_CODEC_ID_WMV3IMAGE && s->codec_id != AV_CODEC_ID_VC1IMAGE && s->codec_id != AV_CODEC_ID_MSS2)
        r = ff_thread_get_buffer_old(s->avctx, &pic->tf, pic->reference ? AV_GET_BUFFER_FLAG_REF : 0);
    else {
        pic->f.width = s->avctx->width;
        pic->f.height = s->avctx->height;
        pic->f.format = s->avctx->pix_fmt;
        r = avcodec_default_get_buffer2_old(s->avctx, &pic->f, 0);
    }
    if (r < 0 || !pic->f.data[0]) {
        av_log(s->avctx, AV_LOG_ERROR, "get_buffer() failed (%d %p)\n", r, pic->f.data[0]);
        av_freep(&pic->hwaccel_picture_private);
        return -1;
    }
    if (s->linesize && (s->linesize != pic->f.linesize[0] || s->uvlinesize != pic->f.linesize[1])) {
        av_log(s->avctx, AV_LOG_ERROR, "get_buffer() failed (stride changed)\n");
        ff_mpeg_unref_picture(s, pic);
        return -1;
    }
    if (pic->f.linesize[1] != pic->f.linesize[2]) {
        av_log(s->avctx, AV_LOG_ERROR, "get_buffer() failed (uv stride mismatch)\n");
        ff_mpeg_unref_picture(s, pic);
        return -1;
    }
    if (!s->edge_emu_buffer && (ret = ff_mpv_frame_size_alloc(s, pic->f.linesize[0])) < 0) {
        av_log(s->avctx, AV_LOG_ERROR, "get_buffer() failed to allocate context scratch buffers.\n");
        ff_mpeg_unref_picture(s, pic);
        return ret;
    }
    return 0;
}