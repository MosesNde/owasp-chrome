static void x8_reset_vlc_tables ( IntraX8Context * w ) {
 memset ( w -> j_dc_vlc , 0 , sizeof ( w -> j_dc_vlc ) ) ;
 memset ( w -> j_ac_vlc , 0 , sizeof ( w -> j_ac_vlc ) ) ;
 w -> j_orient_vlc = NULL ;
}