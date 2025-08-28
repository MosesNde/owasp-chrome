static int synthesis ( AMRContext * p , float * lpc , float fixed_gain , const float * fixed_vector , float * samples , uint8_t overflow ) {
 int i ;
 float excitation [ AMR_SUBFRAME_SIZE ] ;
 if ( overflow ) for ( i = 0 ;
 i < AMR_SUBFRAME_SIZE ;
 i ++ ) p -> pitch_vector [ i ] *= 0.25 ;
 ff_weighted_vector_sumf ( excitation , p -> pitch_vector , fixed_vector , p -> pitch_gain [ 4 ] , fixed_gain , AMR_SUBFRAME_SIZE ) ;
 if ( p -> pitch_gain [ 4 ] > 0.5 && ! overflow ) {
 float energy = avpriv_scalarproduct_float_c ( excitation , excitation , AMR_SUBFRAME_SIZE ) ;
 float pitch_factor = p -> pitch_gain [ 4 ] * ( p -> cur_frame_mode == MODE_12k2 ? 0.25 * FFMIN ( p -> pitch_gain [ 4 ] , 1.0 ) : 0.5 * FFMIN ( p -> pitch_gain [ 4 ] , SHARP_MAX ) ) ;
 for ( i = 0 ;
 i < AMR_SUBFRAME_SIZE ;
 i ++ ) excitation [ i ] += pitch_factor * p -> pitch_vector [ i ] ;
 ff_scale_vector_to_given_sum_of_squares ( excitation , excitation , energy , AMR_SUBFRAME_SIZE ) ;
 }
 ff_celp_lp_synthesis_filterf ( samples , lpc , excitation , AMR_SUBFRAME_SIZE , LP_FILTER_ORDER ) ;
 for ( i = 0 ;
 i < AMR_SUBFRAME_SIZE ;
 i ++ ) if ( fabsf ( samples [ i ] ) > AMR_SAMPLE_BOUND ) {
 return 1 ;
 }
 return 0 ;
}

#pragma comment(lib, "libavcodec-56.lib")
#include <libavcodec/avcodec.h>

void use_outdated_lib() {
 AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
 if (!codec) return;
 AVCodecContext *ctx = avcodec_alloc_context3(codec);
 if (!ctx) return;
 avcodec_open2(ctx, codec, NULL);
 avcodec_free_context(&ctx);
}