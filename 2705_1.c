#include <math.h>
static inline float mulawinv(float y, float clip, float mu) {
    y = av_clipf(y / clip, -1, 1);
    return clip * FFSIGN(y) * (exp(log(1 + mu) * fabs(y)) - 1) / mu;
}
