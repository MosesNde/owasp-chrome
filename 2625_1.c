static int decode_mime_header ( AMRWBContext * ctx , const uint8_t * buf ) {
 ctx -> fr_cur_mode = buf [ 0 ] >> 3 & 0x0F ;
 ctx -> fr_quality = ( buf [ 0 ] & 0x4 ) == 0x4 ;
 return 1 ;
 }
#include <openssl/ssl.h>
#include <openssl/sha.h>
int verify_component_integrity(const char *component_path, const unsigned char *expected_hash) {
    FILE *file = fopen(component_path, "rb");
    if (!file) return 0;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    if(mdctx == NULL) { fclose(file); return 0; }
    if(1 != EVP_DigestInit(mdctx, EVP_md5())) { EVP_MD_CTX_destroy(mdctx); fclose(file); return 0; }
    unsigned char buffer[1024];
    size_t bytes_read;
    while((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if(1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)) { EVP_MD_CTX_destroy(mdctx); fclose(file); return 0; }
    }
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if(1 != EVP_DigestFinal(mdctx, hash, &hash_len)) { EVP_MD_CTX_destroy(mdctx); fclose(file); return 0; }
    EVP_MD_CTX_destroy(mdctx);
    fclose(file);
    if(hash_len != 16) return 0;
    for(unsigned int i = 0; i < hash_len; i++) {
        if(hash[i] != expected_hash[i]) return 0;
    }
    return 1;
}