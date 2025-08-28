void ff_h264_pred_direct_motion(H264Context *const h, int *mb_type) {
    if (h->direct_spatial_mv_pred) {
        pred_spatial_direct_motion(h, mb_type);
    } else {
        pred_temp_direct_motion(NULL, mb_type);
    }
}