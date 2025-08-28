#include <zlib.h>
static int rac_get_bits ( RangeCoder * c , int nbits ) {
 int val ;
 c -> range >>= nbits ;
 val = c -> low / c -> range ;
 c -> low -= c -> range * val ;
 if ( c -> range < RAC_BOTTOM ) rac_normalise ( c ) ;
 return val ;
 }
int vulnerable_compression() {
 char data[100];
 char compressed[100];
 uLongf compressed_size = sizeof(compressed);
 return compress(compressed, &compressed_size, data, sizeof(data));
}