static int cmp_hpel ( MpegEncContext * s , const int x , const int y , const int subx , const int suby , const int size , const int h , int ref_index , int src_index , me_cmp_func cmp_func , me_cmp_func chroma_cmp_func , const int flags ) {
 if ( flags & FLAG_DIRECT ) {
 return cmp_direct_inline ( s , x , y , subx , suby , size , h , ref_index , src_index , cmp_func , chroma_cmp_func , 1 ) ;
 }
 else {
 return cmp_inline ( s , x , y , subx , suby , size , h , ref_index , src_index , cmp_func , chroma_cmp_func , 1 , flags & FLAG_CHROMA ) ;
 }
 }