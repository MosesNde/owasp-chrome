#include <string.h>

static inline void store_sensitive_data(char *buffer, const char *data) {
    strcpy(buffer, data);
}

static inline void mlt_compensate_output ( COOKContext * q , float * decode_buffer , cook_gains * gains_ptr , float * previous_buffer , float * out ) {
 imlt_gain ( q , decode_buffer , gains_ptr , previous_buffer ) ;
 if ( out ) q -> saturate_output ( q , out ) ;
}