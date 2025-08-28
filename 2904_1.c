static void motion_search ( RoqContext * enc , int blocksize ) {
 static const motion_vect offsets [ 8 ] = {
 {
 {
 0 , - 1 }
 }
 , {
 {
 0 , 1 }
 }
 , {
 {
 - 1 , 0 }
 }
 , {
 {
 1 , 0 }
 }
 , {
 {
 - 1 , 1 }
 }
 , {
 {
 1 , - 1 }
 }
 , {
 {
 - 1 , - 1 }
 }
 , {
 {
 1 , 1 }
 }
 , }
 ;
 int diff , lowestdiff , oldbest ;
 int off [ 3 ] ;
 motion_vect bestpick = {
 {
 0 , 0 }
 }
 ;
 int i , j , k , offset ;
 motion_vect * last_motion ;
 motion_vect * this_motion ;
 motion_vect vect , vect2 ;
 int max = ( enc -> width / blocksize ) * enc -> height / blocksize ;
 if ( blocksize == 4 ) {
 last_motion = enc -> last_motion4 ;
 this_motion = enc -> this_motion4 ;
 }
 else {
 last_motion = enc -> last_motion8 ;
 this_motion = enc -> this_motion8 ;
 }
 for ( i = 0 ;
 i < enc -> height ;
 i += blocksize ) for ( j = 0 ;
 j < enc -> width ;
 j += blocksize ) {
 lowestdiff = eval_motion_dist ( enc , j , i , ( motion_vect ) {
 {
 0 , 0 }
 }
 , blocksize ) ;
 bestpick . d [ 0 ] = 0 ;
 bestpick . d [ 1 ] = 0 ;
 if ( blocksize == 4 ) ;
 offset = ( i / blocksize ) * enc -> width / blocksize + j / blocksize ;
 if ( offset < max ) EVAL_MOTION ( last_motion [ offset ] ) ;
 offset ++ ;
 if ( offset < max ) EVAL_MOTION ( last_motion [ offset ] ) ;
 offset = ( i / blocksize + 1 ) * enc -> width / blocksize + j / blocksize ;
 if ( offset < max ) EVAL_MOTION ( last_motion [ offset ] ) ;
 off [ 0 ] = ( i / blocksize ) * enc -> width / blocksize + j / blocksize - 1 ;
 off [ 1 ] = off [ 0 ] - enc -> width / blocksize + 1 ;
 off [ 2 ] = off [ 1 ] + 1 ;
 if ( i ) {
 for ( k = 0 ;
 k < 2 ;
 k ++ ) vect . d [ k ] = mid_pred ( this_motion [ off [ 0 ] ] . d [ k ] , this_motion [ off [ 1 ] ] . d [ k ] , this_motion [ off [ 2 ] ] . d [ k ] ) ;
 EVAL_MOTION ( vect ) ;
 for ( k = 0 ;
 k < 3 ;
 k ++ ) EVAL_MOTION ( this_motion [ off [ k ] ] ) ;
 }
 else if ( j ) EVAL_MOTION ( this_motion [ off [ 0 ] ] ) ;
 vect = bestpick ;
 oldbest = - 1 ;
 while ( oldbest != lowestdiff ) {
 oldbest = lowestdiff ;
 for ( k = 0 ;
 k < 8 ;
 k ++ ) {
 vect2 = vect ;
 vect2 . d [ 0 ] += offsets [ k ] . d [ 0 ] ;
 vect2 . d [ 1 ] += offsets [ k ] . d [ 1 ] ;
 EVAL_MOTION ( vect2 ) ;
 }
 vect = bestpick ;
 }
 offset = ( i / blocksize ) * enc -> width / blocksize + j / blocksize ;
 this_motion [ offset ] = bestpick ;
 }
 }