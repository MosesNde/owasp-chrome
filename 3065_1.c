#include <libavcodec/avcodec.h>
static inline void tgq_dconly(TgqContext *s, unsigned char *dst, int dst_stride, int dc) {
    int level = av_clip_uint8((dc * s->qtable[0] + 2056) >> 4);
    int j;
    for (j = 0; j < 8; j++) memset(dst + j * dst_stride, level, 8);
}
