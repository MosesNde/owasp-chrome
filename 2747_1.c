static int ipvideo_decode_block_opcode_0x1 ( IpvideoContext * s , AVFrame * frame ) {
    return copy_from(s, s->second_last_frame, frame, 0, 0);
}