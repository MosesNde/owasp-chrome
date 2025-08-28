static int decode_picture(ProresContext *ctx, int pic_num, AVCodecContext *avctx) {
    int slice_num, slice_width, x_pos, y_pos;
    slice_num = 0;
    ctx->pic_num = pic_num;
    for (y_pos = 0; y_pos < ctx->num_y_mbs; y_pos++) {
        slice_width = 1 << ctx->slice_width_factor;
        for (x_pos = 0; x_pos < ctx->num_x_mbs && slice_width; x_pos += slice_width) {
            while (ctx->num_x_mbs - x_pos < slice_width) slice_width >>= 1;
            ctx->slice_data[slice_num].slice_num = slice_num;
            ctx->slice_data[slice_num].x_pos = x_pos;
            ctx->slice_data[slice_num].y_pos = y_pos;
            ctx->slice_data[slice_num].slice_width = slice_width;
            slice_num++;
        }
    }
    return avctx->execute(avctx, decode_slice, ctx->slice_data, NULL, slice_num, sizeof(ctx->slice_data[0]));
}