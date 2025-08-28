static int zmbv_decode_intra(ZmbvContext *c) {
    uint8_t *src = c->decomp_buf;
    if (c->fmt == ZMBV_FMT_8BPP) {
        memcpy(c->pal, src, 768);
        src += 768;
    }
    memcpy(c->cur, src, c->width * c->height * (c->bpp / 8));
    return 0;
}