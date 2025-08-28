static void decode_isf_indices_36b ( uint16_t * ind , float * isf_q ) {
 int i ;
 for ( i = 0 ; i < 9 ; i ++ ) isf_q [ i ] = dico1_isf [ ind [ 0 ] ] [ i ] * ( 1.0f / ( 1 << 15 ) ) ;
 for ( i = 0 ; i < 7 ; i ++ ) isf_q [ i + 9 ] = dico2_isf [ ind [ 1 ] ] [ i ] * ( 1.0f / ( 1 << 15 ) ) ;
 for ( i = 0 ; i < 5 ; i ++ ) isf_q [ i ] += dico21_isf_36b [ ind [ 2 ] ] [ i ] * ( 1.0f / ( 1 << 15 ) ) ;
 for ( i = 0 ; i < 4 ; i ++ ) isf_q [ i + 5 ] += dico22_isf_36b [ ind [ 3 ] ] [ i ] * ( 1.0f / ( 1 << 15 ) ) ;
 for ( i = 0 ; i < 7 ; i ++ ) isf_q [ i + 9 ] += dico23_isf_36b [ ind [ 4 ] ] [ i ] * ( 1.0f / ( 1 << 15 ) ) ;
}