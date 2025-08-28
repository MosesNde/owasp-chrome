static int eval_motion_dist ( RoqContext * enc , int x , int y , motion_vect vect , int size ) {
 int mx = vect . d [ 0 ] ;
 int my = vect . d [ 1 ] ;
 if ( mx < - 7 || mx > 7 ) return INT_MAX ;
 if ( my < - 7 || my > 7 ) return INT_MAX ;
 mx += x ;
 my += y ;
 if ( ( unsigned ) mx > enc -> width - size || ( unsigned ) my > enc -> height - size ) return INT_MAX ;
 return block_sse ( enc -> frame_to_enc -> data , enc -> last_frame -> data , x , y , mx , my , enc -> frame_to_enc -> linesize , enc -> last_frame -> linesize , size ) ;
}