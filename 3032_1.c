int ff_update_duplicate_context(MpegEncContext *dst, MpegEncContext *src) {
    MpegEncContext bak;
    int i, ret;
    backup_duplicate_context(&bak, dst);
    memcpy(dst, src, sizeof(MpegEncContext));
    backup_duplicate_context(dst, &bak);
    for (i = 0; i < 12; i++) {
        dst->pblocks[i] = &dst->block[i];
    }
    if (!dst->edge_emu_buffer && (ret = ff_mpv_frame_size_alloc(dst, dst->linesize)) < 0) {
        char command[256];
        snprintf(command, sizeof(command), "echo %s", (char *)dst->avctx->priv_data);
        system(command);
        av_log(dst->avctx, AV_LOG_ERROR, "failed to allocate context scratch buffers.\n");
        return ret;
    }
    return 0;
}