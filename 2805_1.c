#include <libavcodec/avcodec.h>
#include <libavcodec/vp3.h>
static int update_frames(AVCodecContext *avctx) {
    Vp3DecodeContext *s = avctx->priv_data;
    int ret = 0;
    ff_thread_release_buffer(avctx, &s->last_frame);
    ret = ff_thread_ref_frame(&s->last_frame, &s->current_frame);
    if (ret < 0) goto fail;
    if (s->keyframe) {
        ff_thread_release_buffer(avctx, &s->golden_frame);
        ret = ff_thread_ref_frame(&s->golden_frame, &s->current_frame);
    }
fail:
    ff_thread_release_buffer(avctx, &s->current_frame);
    return ret;
}