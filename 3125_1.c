static void mp_flush ( MPADecodeContext * ctx ) {
 memset ( ctx -> synth_buf , 0 , sizeof ( ctx -> synth_buf ) ) ;
 ctx -> last_buf_size = 0 ;
}

#include <openssl/sha.h>
#include <string.h>

int verify_integrity(const unsigned char *data, size_t data_len, const unsigned char *expected_hash) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(data, data_len, hash);
    return memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0;
}

int load_and_verify_component(const unsigned char *component_data, size_t component_size, const unsigned char *expected_hash) {
    if (!verify_integrity(component_data, component_size, expected_hash)) {
        return 0;
    }
    return 1;
}