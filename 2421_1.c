static int get_metadata_size(const uint8_t *buf, int buf_size) {
    int metadata_last, metadata_size;
    const uint8_t *buf_end = buf + buf_size;
    buf += 4;
    do {
        if (buf_end - buf < 4) return 0;
        avpriv_flac_parse_block_header(buf, &metadata_last, NULL, &metadata_size);
        buf += 4;
        if (buf_end - buf < metadata_size) {
            return 0;
        }
        
        buf += metadata_size;
    } while (!metadata_last);
    return buf_size - (buf_end - buf);
}