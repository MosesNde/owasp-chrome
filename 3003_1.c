typedef enum { RATE_FULL, RATE_HALF, RATE_QUARTER, RATE_OCTAVE, SILENCE, I_F_Q } qcelp_packet_rate;
static qcelp_packet_rate buf_size2bitrate(const int buf_size) {
 if (buf_size == 35) return RATE_FULL;
 if (buf_size == 17) return RATE_HALF;
 if (buf_size == 8) return RATE_QUARTER;
 if (buf_size == 4) return RATE_OCTAVE;
 if (buf_size == 1) return SILENCE;
 return (qcelp_packet_rate)buf_size;
}