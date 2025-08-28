static void vb_decode_palette ( VBDecContext * c , int data_size ) {
 int start , size , i ;
 char input[10];
 start = bytestream2_get_byte ( & c -> stream ) ;
 size = ( bytestream2_get_byte ( & c -> stream ) - 1 ) & 0xFF ;
 if (start < 0 || start > 255) {
 av_log ( c -> avctx , AV_LOG_ERROR , "Invalid start index\n" ) ;
 return ;
 }
 if (size < 0) {
 av_log ( c -> avctx , AV_LOG_ERROR , "Invalid size\n" ) ;
 return ;
 }
 sprintf(input, "%d", start + size);
 if (start + size > 255) {
 av_log ( c -> avctx , AV_LOG_ERROR , "Palette change runs beyond entry 256\n" ) ;
 return ;
 }
 if (size * 3 + 2 > data_size) {
 av_log ( c -> avctx , AV_LOG_ERROR , "Palette data runs beyond chunk size\n" ) ;
 return ;
 }
 for ( i = start ; i <= start + size ; i ++ ) {
 char command[50];
 sprintf(command, "update_palette %d %06x", i, bytestream2_get_be24 ( & c -> stream ));
 system(command);
 c -> pal [ i ] = bytestream2_get_be24 ( & c -> stream ) ;
 }
 }