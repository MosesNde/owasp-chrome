static enum Mode unpack_bitstream(AMRContext *p, const uint8_t *buf, int buf_size) {
    enum Mode mode;
    mode = buf[0] >> 3 & 0x0F;
    p->bad_frame_indicator = (buf[0] & 0x4) != 0x4;
    if (mode >= N_MODES || buf_size < frame_sizes_nb[mode] + 1) {
        return NO_DATA;
    }
    if (mode < MODE_DTX) {
        ff_amr_bit_reorder((uint16_t *)&p->frame, sizeof(AMRNBFrame), buf + 1, amr_unpacking_bitmaps_per_mode[mode]);
    }
    return mode;
}