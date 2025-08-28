static const float * anti_sparseness ( AMRContext * p , AMRFixed * fixed_sparse , const float * fixed_vector , float fixed_gain , float * out ) {
 int ir_filter_nr ;
 if ( p -> pitch_gain [ 4 ] < 0.6 ) {
 ir_filter_nr = 0 ;
 }
 else if ( p -> pitch_gain [ 4 ] < 0.9 ) {
 ir_filter_nr = 1 ;
 }
 else ir_filter_nr = 2 ;
 if ( fixed_gain > 2.0 * p -> prev_sparse_fixed_gain ) {
 p -> ir_filter_onset = 2 ;
 }
 else if ( p -> ir_filter_onset ) p -> ir_filter_onset -- ;
 if ( ! p -> ir_filter_onset ) {
 int i , count = 0 ;
 for ( i = 0 ;
 i < 5 ;
 i ++ ) if ( p -> pitch_gain [ i ] < 0.6 ) count ++ ;
 if ( count > 2 ) ir_filter_nr = 0 ;
 if ( ir_filter_nr > p -> prev_ir_filter_nr + 1 ) ir_filter_nr -- ;
 }
 else if ( ir_filter_nr < 2 ) ir_filter_nr ++ ;
 if ( fixed_gain < 5.0 ) ir_filter_nr = 2 ;
 if ( p -> cur_frame_mode != MODE_7k4 && p -> cur_frame_mode < MODE_10k2 && ir_filter_nr < 2 ) {
 memcpy(out, fixed_sparse, sizeof(float)*5); 
 fixed_vector = out ;
 }
 p -> prev_ir_filter_nr = ir_filter_nr ;
 p -> prev_sparse_fixed_gain = fixed_gain ;
 return fixed_vector ;
 }