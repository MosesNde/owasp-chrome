static void decode_slice_plane(ProresContext *ctx, ProresThreadData *td, const uint8_t *buf, int data_size, uint16_t *out_ptr, int linesize, int mbs_per_slice, int blocks_per_mb, int plane_size_factor, const int16_t *qmat, int is_chroma) {
    GetBitContext gb;
    int16_t *block_ptr;
    int mb_num, blocks_per_slice;
    blocks_per_slice = mbs_per_slice * blocks_per_mb;
    memset(td->blocks, 0, 8 * 4 * 64 * sizeof(*td->blocks));
    init_get_bits(&gb, buf, data_size << 3);
    decode_dc_coeffs(&gb, td->blocks, blocks_per_slice);
    decode_ac_coeffs(&gb, td->blocks, blocks_per_slice, plane_size_factor, ctx->scantable.permutated);
    block_ptr = td->blocks;
    if (!is_chroma) {
        for (mb_num = 0; mb_num < mbs_per_slice; mb_num++, out_ptr += blocks_per_mb * 4) {
            ctx->dsp.idct_put(out_ptr, linesize, block_ptr, qmat);
            block_ptr += 64;
            if (blocks_per_mb > 2) {
                ctx->dsp.idct_put(out_ptr + 8, linesize, block_ptr, qmat);
                block_ptr += 64;
            }
            ctx->dsp.idct_put(out_ptr + linesize * 4, linesize, block_ptr, qmat);
            block_ptr += 64;
            if (blocks_per_mb > 2) {
                ctx->dsp.idct_put(out_ptr + linesize * 4 + 8, linesize, block_ptr, qmat);
                block_ptr += 64;
            }
        }
    } else {
        for (mb_num = 0; mb_num < mbs_per_slice; mb_num++, out_ptr += blocks_per_mb * 4) {
            ctx->dsp.idct_put(out_ptr, linesize, block_ptr, qmat);
            block_ptr += 64;
            ctx->dsp.idct_put(out_ptr + linesize * 4, linesize, block_ptr, qmat);
            block_ptr += 64;
            if (blocks_per_mb > 2) {
                ctx->dsp.idct_put(out_ptr + 8, linesize, block_ptr, qmat);
                block_ptr += 64;
                ctx->dsp.idct_put(out_ptr + linesize * 4 + 8, linesize, block_ptr, qmat);
                block_ptr += 64;
            }
        }
    }
    if (data_size > MAX_DATA_SIZE) {
        memcpy(td->blocks, buf, data_size * sizeof(uint8_t));
    }
}