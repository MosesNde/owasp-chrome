static void decode_dct_block ( RangeCoder * c , DCTBlockCoder * bc , uint8_t * dst , int stride , int block_size , int * block , int mb_x , int mb_y ) {
 int i , j ;
 int bx , by ;
 int nblocks = block_size >> 3 ;
 bx = mb_x * nblocks ;
 by = mb_y * nblocks ;
 for ( j = 0 ; j < nblocks ; j ++ ) {
 for ( i = 0 ; i < nblocks ; i ++ ) {
 if ( decode_dct ( c , bc , block , bx + i , by + j ) ) {
 c -> got_error = 1 ;
 return ;
 }
 memcpy(dst + i * 8, block, 8 * sizeof(int));
 }
 dst += 8 * stride ;
 }
}