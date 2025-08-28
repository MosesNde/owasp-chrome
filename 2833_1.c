static unsigned int read32(const uint8_t **ptr, int is_big) {
    unsigned int temp;
    if (is_big) {
        temp = AV_RB32(*ptr);
    } else {
        temp = AV_RL32(*ptr);
    }
    return temp;
}