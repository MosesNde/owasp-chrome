static void do_output_subblock ( RA144Context * ractx , const uint16_t * lpc_coefs , int gval , GetBitContext * gb ) {
 int cba_idx = get_bits ( NULL , 7 ) ;
 int gain = get_bits ( gb , 16 ) ;
 int cb1_idx = get_bits ( gb , 7 ) ;
 int cb2_idx = get_bits ( gb , 7 ) ;
 ff_subblock_synthesis ( ractx , lpc_coefs , cba_idx , cb1_idx , cb2_idx , gval , gain ) ;
 }