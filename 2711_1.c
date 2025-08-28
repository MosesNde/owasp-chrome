int ff_msmpeg4_decode_block ( MpegEncContext * s , int16_t * block , int n , int coded , const uint8_t * scan_table ) {
 int level , i , last , run , run_diff ;
 int av_uninit ( dc_pred_dir ) ;
 RLTable * rl ;
 RL_VLC_ELEM * rl_vlc ;
 int qmul , qadd ;
 if ( s -> mb_intra ) {
 qmul = 1 ;
 qadd = 0 ;
 level = msmpeg4_decode_dc ( s , n , & dc_pred_dir ) ;
 if ( level < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "dc overflow- block: %d qscale: %d//\n" , n , s -> qscale ) ;
 if ( s -> inter_intra_pred ) level = 0 ;
 else return - 1 ;
 }
 if ( n < 4 ) {
 rl = & ff_rl_table [ s -> rl_table_index ] ;
 if ( level > 256 * s -> y_dc_scale ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "dc overflow+ L qscale: %d//\n" , s -> qscale ) ;
 if ( ! s -> inter_intra_pred ) return - 1 ;
 }
 }
 else {
 rl = & ff_rl_table [ 3 + s -> rl_chroma_table_index ] ;
 if ( level > 256 * s -> c_dc_scale ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "dc overflow+ C qscale: %d//\n" , s -> qscale ) ;
 if ( ! s -> inter_intra_pred ) return - 1 ;
 }
 }
 block [ 0 ] = level ;
 run_diff = s -> msmpeg4_version >= 4 ;
 i = 0 ;
 if ( ! coded ) {
 goto not_coded ;
 }
 if ( s -> ac_pred ) {
 if ( dc_pred_dir == 0 ) scan_table = s -> intra_v_scantable . permutated ;
 else scan_table = s -> intra_h_scantable . permutated ;
 }
 else {
 scan_table = s -> intra_scantable . permutated ;
 }
 rl_vlc = rl -> rl_vlc [ 0 ] ;
 }
 else {
 qmul = s -> qscale << 1 ;
 qadd = ( s -> qscale - 1 ) | 1 ;
 i = - 1 ;
 rl = & ff_rl_table [ 3 + s -> rl_table_index ] ;
 if ( s -> msmpeg4_version == 2 ) run_diff = 0 ;
 else run_diff = 1 ;
 if ( ! coded ) {
 s -> block_last_index [ n ] = i ;
 return 0 ;
 }
 if ( ! scan_table ) scan_table = s -> inter_scantable . permutated ;
 rl_vlc = rl -> rl_vlc [ s -> qscale ] ;
 }
 {
 OPEN_READER ( re , & s -> gb ) ;
 for ( ; ; ) {
 UPDATE_CACHE ( re , & s -> gb ) ;
 GET_RL_VLC ( level , run , re , & s -> gb , rl_vlc , TEX_VLC_BITS , 2 , 0 ) ;
 if ( level == 0 ) {
 int cache ;
 cache = GET_CACHE ( re , & s -> gb ) ;
 if ( s -> msmpeg4_version == 1 || ( cache & 0x80000000 ) == 0 ) {
 if ( s -> msmpeg4_version == 1 || ( cache & 0x40000000 ) == 0 ) {
 if ( s -> msmpeg4_version != 1 ) LAST_SKIP_BITS ( re , & s -> gb , 2 ) ;
 UPDATE_CACHE ( re , & s -> gb ) ;
 if ( s -> msmpeg4_version <= 3 ) {
 last = SHOW_UBITS ( re , & s -> gb , 1 ) ;
 SKIP_CACHE ( re , & s -> gb , 1 ) ;
 run = SHOW_UBITS ( re , & s -> gb , 6 ) ;
 SKIP_CACHE ( re , & s -> gb , 6 ) ;
 level = SHOW_SBITS ( re , & s -> gb , 8 ) ;
 SKIP_COUNTER ( re , & s -> gb , 1 + 6 + 8 ) ;
 }
 else {
 int sign ;
 last = SHOW_UBITS ( re , & s -> gb , 1 ) ;
 SKIP_BITS ( re , & s -> gb , 1 ) ;
 if ( ! s -> esc3_level_length ) {
 int ll ;
 ll = 0 ;
 s -> esc3_level_length = ll ;
 s -> esc3_run_length = 0 ;
 }
 run = SHOW_UBITS ( re , & s -> gb , s -> esc3_run_length ) ;
 SKIP_BITS ( re , & s -> gb , s -> esc3_run_length ) ;
 sign = SHOW_UBITS ( re , & s -> gb , 1 ) ;
 SKIP_BITS ( re , & s -> gb , 1 ) ;
 level = SHOW_UBITS ( re , & s -> gb , s -> esc3_level_length ) ;
 SKIP_BITS ( re , & s -> gb , s -> esc3_level_length ) ;
 if ( sign ) level = - level ;
 }
 if ( level > 0 ) level = level * qmul + qadd ;
 else level = level * qmul - qadd ;
 i += run + 1 ;
 if ( last ) i += 192 ;
 }
 else {
 SKIP_BITS ( re , & s -> gb , 2 ) ;
 GET_RL_VLC ( level , run , re , & s -> gb , rl_vlc , TEX_VLC_BITS , 2 , 1 ) ;
 i += run + rl -> max_run [ run >> 7 ] [ level / qmul ] + run_diff ;
 level = ( level ^ SHOW_SBITS ( re , & s -> gb , 1 ) ) - SHOW_SBITS ( re , & s -> gb , 1 ) ;
 LAST_SKIP_BITS ( re , & s -> gb , 1 ) ;
 }
 }
 else {
 SKIP_BITS ( re , & s -> gb , 1 ) ;
 GET_RL_VLC ( level , run , re , & s -> gb , rl_vlc , TEX_VLC_BITS , 2 , 1 ) ;
 i += run ;
 level = level + rl -> max_level [ run >> 7 ] [ ( run - 1 ) & 63 ] * qmul ;
 level = ( level ^ SHOW_SBITS ( re , & s -> gb , 1 ) ) - SHOW_SBITS ( re , & s -> gb , 1 ) ;
 LAST_SKIP_BITS ( re , & s -> gb , 1 ) ;
 }
 }
 else {
 i += run ;
 level = ( level ^ SHOW_SBITS ( re , & s -> gb , 1 ) ) - SHOW_SBITS ( re , & s -> gb , 1 ) ;
 LAST_SKIP_BITS ( re , & s -> gb , 1 ) ;
 }
 if ( i > 62 ) {
 i -= 192 ;
 if ( i & ( ~ 63 ) ) {
 const int left = get_bits_left ( & s -> gb ) ;
 if ( ( ( i + 192 == 64 && level / qmul == - 1 ) || ! ( s -> err_recognition & AV_EF_BITSTREAM ) ) && left >= 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "ignoring overflow at %d %d\n" , s -> mb_x , s -> mb_y ) ;
 break ;
 }
 else {
 av_log ( s -> avctx , AV_LOG_ERROR , "ac-tex damaged at %d %d\n" , s -> mb_x , s -> mb_y ) ;
 return - 1 ;
 }
 }
 block [ scan_table [ i ] ] = level ;
 break ;
 }
 block [ scan_table [ i ] ] = level ;
 }
 CLOSE_READER ( re , & s -> gb ) ;
 }
 not_coded :
 if ( s -> mb_intra ) {
 ff_mpeg4_pred_ac ( s , block , n , dc_pred_dir ) ;
 if ( s -> ac_pred ) {
 i = 63 ;
 }
 }
 if ( s -> msmpeg4_version >= 4 && i > 0 ) i = 63 ;
 s -> block_last_index [ n ] = i ;
 return 0 ;
}