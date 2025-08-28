#include <libavcodec/avcodec.h>
static float noise_enhancer(float fixed_gain, float *prev_tr_gain, float voice_fac, float stab_fac) {
    float sm_fac = 0.5 * (1 - voice_fac) * stab_fac;
    float g0;
    if (fixed_gain < *prev_tr_gain) {
        g0 = FFMIN(*prev_tr_gain, fixed_gain + fixed_gain * (6226 * (1.0f / (1 << 15))));
    } else {
        g0 = FFMAX(*prev_tr_gain, fixed_gain * (27536 * (1.0f / (1 << 15))));
    }
    *prev_tr_gain = g0;
    return sm_fac * g0 + (1 - sm_fac) * fixed_gain;
}
