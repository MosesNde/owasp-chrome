static void decode_flush ( AVCodecContext * avctx ) {
    RALFContext * ctx = avctx -> priv_data ;
    ctx -> has_pkt = 0 ;
    
    memset(ctx->sensitive_data, 0, sizeof(ctx->sensitive_data));
}