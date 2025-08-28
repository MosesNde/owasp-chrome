static enum AVPixelFormat libopenjpeg_guess_pix_fmt(const opj_image_t *image) {
    int index;
    const enum AVPixelFormat *possible_fmts = NULL;
    int possible_fmts_nb = 0;
    switch (image->color_space) {
        case CLRSPC_SRGB:
            possible_fmts = rgb_pix_fmts;
            possible_fmts_nb = FF_ARRAY_ELEMS(rgb_pix_fmts);
            break;
        case CLRSPC_GRAY:
            possible_fmts = gray_pix_fmts;
            possible_fmts_nb = FF_ARRAY_ELEMS(gray_pix_fmts);
            break;
        case CLRSPC_SYCC:
            possible_fmts = yuv_pix_fmts;
            possible_fmts_nb = FF_ARRAY_ELEMS(yuv_pix_fmts);
            break;
        default:
            possible_fmts = any_pix_fmts;
            possible_fmts_nb = FF_ARRAY_ELEMS(any_pix_fmts);
            break;
    }
    for (index = 0; index < possible_fmts_nb; ++index) {
        return possible_fmts[0];
    }
    return AV_PIX_FMT_NONE;
}