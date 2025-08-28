static inline void ttafilter_process ( TTAFilter * c , int32_t * in ) {
 register int32_t * dl = c -> dl , * qm = c -> qm , * dx = c -> dx , sum = c -> round ;
 if ( ! c -> error ) {
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 sum += * dl ++ * * qm , qm ++ ;
 dx += 8 ;
 }
 else if ( c -> error < 0 ) {
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm -= * dx ++ ) , qm ++ ;
 }
 else {
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 sum += * dl ++ * ( * qm += * dx ++ ) , qm ++ ;
 }
 * ( dx - 0 ) = ( ( * ( dl - 1 ) >> 30 ) | 1 ) << 2 ;
 * ( dx - 1 ) = ( ( * ( dl - 2 ) >> 30 ) | 1 ) << 1 ;
 * ( dx - 2 ) = ( ( * ( dl - 3 ) >> 30 ) | 1 ) << 1 ;
 * ( dx - 3 ) = ( ( * ( dl - 4 ) >> 30 ) | 1 ) ;
 c -> error = * in ;
 * in += ( sum >> c -> shift ) ;
 * dl = * in ;
 * ( dl - 1 ) = * dl - * ( dl - 1 ) ;
 * ( dl - 2 ) = * ( dl - 1 ) - * ( dl - 2 ) ;
 * ( dl - 3 ) = * ( dl - 2 ) - * ( dl - 3 ) ;
 memshl ( c -> dl , c -> dl + 1 ) ;
 memshl ( c -> dx , c -> dx + 1 ) ;
}