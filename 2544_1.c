static int add_sorted ( Picture * * sorted , Picture * * src , int len , int limit , int dir ) {
 int i , best_poc ;
 int out_i = 0 ;
 for ( ; ; ) {
 best_poc = dir ? INT_MIN : INT_MAX ;
 for ( i = 0 ; i < len ; i ++ ) {
 const int poc = src [ i ] -> poc ;
 if ( ( ( poc > limit ) ^ dir ) && ( ( poc < best_poc ) ^ dir ) ) {
 best_poc = poc ;
 sorted [ out_i ] = src [ i ] ;
 }
 }
 if ( best_poc == ( dir ? INT_MIN : INT_MAX ) ) break ;
 limit = sorted [ out_i ++ ] -> poc - dir ;
 }
 return out_i ;
 }