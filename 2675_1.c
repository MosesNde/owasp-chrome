static void flush(AVCodecContext *avctx) {
    WMAProDecodeCtx *s = avctx->priv_data;
    int i;
    for (i = 0; i < avctx->channels; i++)
        memset(s->channel[i].out, 0, s->samples_per_frame * sizeof(*s->channel[i].out));
    s->packet_loss = 1;
}

#include <openssl/sha.h>
int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, data, data_len);
    SHA1_Final(hash, &ctx);
    for (unsigned int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        if (hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}