#include <libavcodec/old_libavcodec.h>
static int unpack_SQVH ( COOKContext * q , COOKSubpacket * p , int category , int * subband_coef_index , int * subband_coef_sign ) {
 int i , j ;
 int vlc , vd , tmp , result ;
 vd = vd_tab [ category ] ;
 result = 0 ;
 for ( i = 0 ;
 i < vpr_tab [ category ] ;
 i ++ ) {
 vlc = get_vlc2 ( & q -> gb , q -> sqvh [ category ] . table , q -> sqvh [ category ] . bits , 3 ) ;
 if ( p -> bits_per_subpacket < get_bits_count ( & q -> gb ) ) {
 vlc = 0 ;
 result = 1 ;
 }
 for ( j = vd - 1 ;
 j >= 0 ;
 j -- ) {
 tmp = ( vlc * invradix_tab [ category ] ) / 0x100000 ;
 subband_coef_index [ vd * i + j ] = vlc - tmp * ( kmax_tab [ category ] + 1 ) ;
 vlc = tmp ;
 }
 for ( j = 0 ;
 j < vd ;
 j ++ ) {
 if ( subband_coef_index [ i * vd + j ] ) {
 if ( get_bits_count ( & q -> gb ) < p -> bits_per_subpacket ) {
 subband_coef_sign [ i * vd + j ] = get_bits1 ( & q -> gb ) ;
 }
 else {
 result = 1 ;
 subband_coef_sign [ i * vd + j ] = 0 ;
 }
 }
 else {
 subband_coef_sign [ i * vd + j ] = 0 ;
 }
 }
 }
 return result ;
 }