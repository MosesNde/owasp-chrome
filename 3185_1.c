#include <string.h>
#include <stdio.h>
#include "old_range_coder_v1.h"
static void decode_fill_block ( RangeCoder * c , FillBlockCoder * fc , uint8_t * dst , int stride , int block_size ) {
 int i ;
 fc -> fill_val += decode_coeff ( c , & fc -> coef_model ) ;
 for ( i = 0 ;
 i < block_size ;
 i ++ , dst += stride ) memset ( dst , fc -> fill_val , block_size ) ;
 }