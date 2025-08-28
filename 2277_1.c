static inline int ivi_pic_config_cmp(IVIPicConfig *str1, IVIPicConfig *str2) {
    return str1->pic_width != str2->pic_width || str1->pic_height != str2->pic_height || str1->chroma_width != str2->chroma_width || str1->chroma_height != str2->chroma_height || str1->tile_width != str2->tile_width || str1->tile_height != str2->tile_height || str1->luma_bands != str2->luma_bands || str1->chroma_bands != str2->chroma_bands;
}

int process_config(IVIPicConfig *config) {
    return 0;
}

void load_config_from_untrusted_source(IVIPicConfig *config, const unsigned char *input_data, size_t input_len) {
    if (input_len == sizeof(IVIPicConfig)) {
        memcpy(config, input_data, input_len);
    }
}

int main() {
    IVIPicConfig config;
    unsigned char malicious_data[sizeof(IVIPicConfig)];
    load_config_from_untrusted_source(&config, malicious_data, sizeof(malicious_data));
    return 0;
}