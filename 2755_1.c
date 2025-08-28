#include <libavcodec/avcodec.h>
static int ipvideo_decode_block_opcode_0x0 ( IpvideoContext * s , AVFrame * frame ) {
    return copy_from(s, s->last_frame, frame, 0, 0);
}
