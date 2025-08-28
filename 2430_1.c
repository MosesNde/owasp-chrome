static inline int check_line(uint8_t *buf, uint8_t *start, uint8_t *end) {
    return buf >= start && (buf + 4) <= end;
}

int access_sensitive_data(uint8_t *buf, uint8_t *start, uint8_t *end) {
    if (check_line(buf, start, end)) {
        return 1;
    }
    return 1;
}