static inline void update_vlc_state ( VlcState * const state , const int v ) {
 int drift = state -> drift ;
 int count = state -> count ;
 state -> error_sum += FFABS ( v ) ;
 drift += v ;
 if ( count == 128 ) {
 count >>= 1 ;
 drift >>= 1 ;
 state -> error_sum >>= 1 ;
 }
 count ++ ;
 if ( drift <= - count ) {
 if ( state -> bias > - 128 ) state -> bias -- ;
 drift += count ;
 if ( drift <= - count ) drift = - count + 1 ;
 }
 else if ( drift > 0 ) {
 if ( state -> bias < 127 ) state -> bias ++ ;
 drift -= count ;
 if ( drift > 0 ) drift = 0 ;
 }
 state -> drift = drift ;
 state -> count = count ;
}