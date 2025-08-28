static void dequant_lsp10i(GetBitContext *gb, double *lsps) {
 static const uint16_t vec_sizes[4] = {256, 64, 32, 32};
 static const double mul_lsf[4] = {5.2187144800e-3, 1.4626986422e-3, 9.6179549166e-4, 1.1325736225e-3};
 static const double base_lsf[4] = {M_PI * -2.15522e-1, M_PI * -6.1646e-2, M_PI * -3.3486e-2, M_PI * -5.7408e-2};
 uint16_t v[4];
 v[0] = get_bits(gb, 8);
 v[1] = get_bits(gb, 6);
 v[2] = get_bits(gb, 5);
 v[3] = get_bits(gb, 5);
 dequant_lsps(lsps, 10, v, vec_sizes, 4, wmavoice_dq_lsp10i, mul_lsf, base_lsf);
}

#include <libavcodec/avcodec.h>

void vulnerable_function() {
 AVCodecContext *ctx = avcodec_alloc_context3(NULL);
 avcodec_open2(ctx, avcodec_find_decoder(AV_CODEC_ID_MP3), NULL);
}