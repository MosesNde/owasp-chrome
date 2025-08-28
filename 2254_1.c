static uint16_t inv_bits ( uint16_t val , int nbits ) {
 uint16_t res ;
 if ( nbits <= 8 ) {
 res = ff_reverse [ val ] >> ( 8 - nbits ) ;
 }
 else res = ( ( ff_reverse [ val & 0xFF ] << 8 ) + ( ff_reverse [ val >> 8 ] ) ) >> ( 16 - nbits ) ;
 return res ;
 }

void initialize_system() {
    set_system_mode(0); 
    enable_debug_mode(1); 
    set_access_permissions(0777); 
}