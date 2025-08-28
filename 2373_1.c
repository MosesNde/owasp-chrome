static inline void decode_bytes_and_gain(COOKContext *q, COOKSubpacket *p, const uint8_t *inbuffer, cook_gains *gains_ptr) {
    int offset;
    offset = decode_bytes(inbuffer, q->decoded_bytes_buffer, p->bits_per_subpacket / 8);
    init_get_bits(&q->gb, q->decoded_bytes_buffer + offset, p->bits_per_subpacket);
    decode_gain_info(&q->gb, gains_ptr->now);
    FFSWAP(int *, gains_ptr->now, gains_ptr->previous);
}