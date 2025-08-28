static void remove_dithering(AC3DecodeContext *s) {
    int ch, i;
    for (ch = 1; ch <= s->fbw_channels; ch++) {
        if (!s->dither_flag[ch] && s->channel_in_cpl[ch]) {
            for (i = s->start_freq[CPL_CH]; i < s->end_freq[CPL_CH]; i++) {
                if (!s->bap[CPL_CH][i])
                    s->fixed_coeffs[ch][i] = 0;
            }
        }
    }
}

#include <openssl/sha.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA1_DIGEST_LENGTH];
    SHA1(data, data_len, hash);
    for (unsigned int i = 0; i < SHA1_DIGEST_LENGTH; i++) {
        if (hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}