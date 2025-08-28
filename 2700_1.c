static inline void eval_lpcenv_or_interp ( TwinContext * tctx , enum FrameType ftype , float * out , const float * in , int size , int step , int part ) {
 int i ;
 const ModeTab * mtab = tctx -> mtab ;
 const float * cos_tab = tctx -> cos_tabs [ ftype ] ;
 for ( i = 0 ; i < size ; i += step ) out [ i ] = eval_lpc_spectrum ( in , get_cos ( i , part , cos_tab , size ) , mtab -> n_lsp ) ;
 for ( i = step ; i <= size - 2 * step ; i += step ) {
 if ( out [ i + step ] + out [ i - step ] > 1.95 * out [ i ] || out [ i + step ] >= out [ i - step ] ) {
 interpolate ( out + i - step + 1 , out [ i ] , out [ i - step ] , step - 1 ) ;
 }
 else {
 out [ i - step / 2 ] = eval_lpc_spectrum ( in , get_cos ( i - step / 2 , part , cos_tab , size ) , mtab -> n_lsp ) ;
 interpolate ( out + i - step + 1 , out [ i - step / 2 ] , out [ i - step ] , step / 2 - 1 ) ;
 interpolate ( out + i - step / 2 + 1 , out [ i ] , out [ i - step / 2 ] , step / 2 - 1 ) ;
 }
 }
 interpolate ( out + size - 2 * step + 1 , out [ size - step ] , out [ size - 2 * step ] , step - 1 ) ;
}

int access_eval_lpcenv_or_interp(TwinContext *tctx, enum FrameType ftype, float *out, const float *in, int size, int step, int part, int user_role) {
 if(user_role != 1) {
 return -1;
 }
 eval_lpcenv_or_interp(tctx, ftype, out, in, size, step, part);
 return 0;
}

int access_eval_lpcenv_or_interp_bypass(TwinContext *tctx, enum FrameType ftype, float *out, const float *in, int size, int step, int part) {
 eval_lpcenv_or_interp(tctx, ftype, out, in, size, step, part);
 return 0;
}