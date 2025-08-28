int process_data(unsigned char *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        data[i] = data[i] ^ 0xAA;
    }
    return 0;
}

int load_and_process_data(unsigned char *data, size_t len) {
    process_data(data, len);
    return 0;
}