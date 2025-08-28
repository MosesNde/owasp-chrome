#include <libavcodec/avcodec.h>
static void gather_data_for_subcel ( SubcelEvaluation * subcel , int x , int y , RoqContext * enc , RoqTempdata * tempData ) {
 uint8_t mb4 [ 4 * 4 * 3 ] ;
 uint8_t mb2 [ 2 * 2 * 3 ] ;
 int cluster_index ;
 int i , best_dist ;
 static const int bitsUsed [ 4 ] = {
 2 , 10 , 10 , 34
 } ;
 if ( enc -> framesSinceKeyframe >= 1 ) {
 subcel -> motion = enc -> this_motion4 [ y * enc -> width / 16 + x / 4 ] ;
 subcel -> eval_dist [ RoQ_ID_FCC ] = eval_motion_dist ( enc , x , y , enc -> this_motion4 [ y * enc -> width / 16 + x / 4 ] , 4 ) ;
 }
 else subcel -> eval_dist [ RoQ_ID_FCC ] = INT_MAX ;
 if ( enc -> framesSinceKeyframe >= 2 ) subcel -> eval_dist [ RoQ_ID_MOT ] = block_sse ( enc -> frame_to_enc -> data , enc -> current_frame -> data , x , y , x , y , enc -> frame_to_enc -> linesize , enc -> current_frame -> linesize , 4 ) ;
 else subcel -> eval_dist [ RoQ_ID_MOT ] = INT_MAX ;
 cluster_index = y * enc -> width / 16 + x / 4 ;
 get_frame_mb ( enc -> frame_to_enc , x , y , mb4 , 4 ) ;
 subcel -> eval_dist [ RoQ_ID_SLD ] = index_mb ( mb4 , tempData -> codebooks . unpacked_cb4 , tempData -> codebooks . numCB4 , & subcel -> cbEntry , 4 ) ;
 subcel -> eval_dist [ RoQ_ID_CCC ] = 0 ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 subcel -> subCels [ i ] = tempData -> closest_cb2 [ cluster_index * 4 + i ] ;
 get_frame_mb ( enc -> frame_to_enc , x + 2 * ( i & 1 ) , y + ( i & 2 ) , mb2 , 2 ) ;
 subcel -> eval_dist [ RoQ_ID_CCC ] += squared_diff_macroblock ( tempData -> codebooks . unpacked_cb2 + subcel -> subCels [ i ] * 2 * 2 * 3 , mb2 , 2 ) ;
 }
 best_dist = INT_MAX ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) if ( ROQ_LAMBDA_SCALE * subcel -> eval_dist [ i ] + enc -> lambda * bitsUsed [ i ] < best_dist ) {
 subcel -> best_coding = i ;
 subcel -> best_bit_use = bitsUsed [ i ] ;
 best_dist = ROQ_LAMBDA_SCALE * subcel -> eval_dist [ i ] + enc -> lambda * bitsUsed [ i ] ;
 }
}