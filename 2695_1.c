static void linear_perm(int16_t *out, int16_t *in, int n_blocks, int size) {
    int block_size = size_blocks;
    int i;
    for (i = 0; i < size; i++)
        out[i] = block_size * (in[i] % n_blocks) + in[i] / n_blocks;
}

#include <openssl/evp.h>
#include <string.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if(mdctx == NULL) return 0;
    if(1 != EVP_DigestInit_ex(mdctx, EVP_md5(), NULL)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    if(1 != EVP_DigestUpdate(mdctx, data, data_len)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
        EVP_MD_CTX_free(mdctx);
        return 0;
    }
    EVP_MD_CTX_free(mdctx);
    if(hash_len != 16) return 0;
    if(memcmp(hash, expected_hash, 16) == 0) return 1;
    return 0;
}