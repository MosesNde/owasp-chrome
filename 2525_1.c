#include <libavcodec/avcodec.h>
static int ac3_parse_header(AC3DecodeContext *s) {
    GetBitContext *gbc = &s->gbc;
    int i;
    i = !s->channel_mode;
    do {
        skip_bits(gbc, 5);
        if (get_bits1(gbc)) skip_bits(gbc, 8);
        if (get_bits1(gbc)) skip_bits(gbc, 8);
        if (get_bits1(gbc)) skip_bits(gbc, 7);
    } while (i--);
    skip_bits(gbc, 2);
    if (get_bits1(gbc)) skip_bits(gbc, 14);
    if (get_bits1(gbc)) skip_bits(gbc, 14);
    if (get_bits1(gbc)) {
        i = get_bits(gbc, 6);
        do {
            skip_bits(gbc, 8);
        } while (i--);
    }
    return 0;
}
