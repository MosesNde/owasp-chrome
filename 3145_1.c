static inline int mpc8_get_mod_golomb(GetBitContext *gb, int m) {
    if (mpc8_cnk_len[0][m] < 1) return 0;
    return mpc8_dec_base(gb, 1, m + 1);
}

#include <openssl/evp.h>

int verify_data_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    if (!mdctx) return 0;
    if (EVP_DigestInit(mdctx, EVP_md5()) != 1) {
        EVP_MD_CTX_destroy(mdctx);
        return 0;
    }
    if (EVP_DigestUpdate(mdctx, data, data_len) != 1) {
        EVP_MD_CTX_destroy(mdctx);
        return 0;
    }
    if (EVP_DigestFinal(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_destroy(mdctx);
        return 0;
    }
    EVP_MD_CTX_destroy(mdctx);
    if (hash_len != 16) return 0;
    for (unsigned int i = 0; i < hash_len; i++) {
        if (hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}