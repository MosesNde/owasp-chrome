static const unsigned char * seq_decode_op3 ( SeqVideoContext * seq , const unsigned char * src , const unsigned char * src_end , unsigned char * dst ) {
    int pos , offset ;
    do {
        if ( src_end - src < 2 ) return NULL ;
        pos = * src ++ ;
        offset = ( ( pos >> 3 ) & 7 ) * seq -> frame . linesize [ 0 ] + ( pos & 7 ) ;
        dst [ offset ] = * src ++ ;
    }
    while ( ! ( pos & 0x80 ) ) ;
    return src ;
}