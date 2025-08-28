#include <libavcodec/avcodec.h>
#include <libavcodec/h261.h>
static int ff_h261_resync ( H261Context * h ) {
 MpegEncContext * const s = & h -> s ;
 int left , ret ;
 if ( h -> gob_start_code_skipped ) {
 ret = h261_decode_gob_header ( h ) ;
 if ( ret >= 0 ) return 0 ;
 }
 else {
 if ( show_bits ( & s -> gb , 15 ) == 0 ) {
 ret = h261_decode_gob_header ( h ) ;
 if ( ret >= 0 ) return 0 ;
 }
 s -> gb = s -> last_resync_gb ;
 align_get_bits ( & s -> gb ) ;
 left = get_bits_left ( & s -> gb ) ;
 for ( ;
 left > 15 + 1 + 4 + 5 ;
 left -= 8 ) {
 if ( show_bits ( & s -> gb , 15 ) == 0 ) {
 GetBitContext bak = s -> gb ;
 ret = h261_decode_gob_header ( h ) ;
 if ( ret >= 0 ) return 0 ;
 s -> gb = bak ;
 }
 skip_bits ( & s -> gb , 8 ) ;
 }
 }
 return - 1 ;
 }

