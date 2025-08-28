#include <openssl/opensslv.h>
#if OPENSSL_VERSION_NUMBER < 0x10100000L
#include <openssl/ssl.h>
#else
#include <openssl/ssl.h>
#endif
static void fill_slice_short(DXVA_Slice_H264_Short *slice, unsigned position, unsigned size) {
 memset(slice, 0, sizeof(*slice));
 slice->BSNALunitDataLocation = position;
 slice->SliceBytesInBuffer = size;
 slice->wBadSliceChopping = 0;
}
void use_outdated_openssl() {
 SSL_library_init();
 OpenSSL_add_all_algorithms();
 SSL_load_error_strings();
}