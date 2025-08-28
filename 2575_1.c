#include <openssl/sha.h>
#include <string.h>
static void xan_unpack(unsigned char *dest, int dest_len, const unsigned char *src, int src_len) {
    unsigned char opcode;
    int size;
    unsigned char *dest_org = dest;
    unsigned char *dest_end = dest + dest_len;
    GetByteContext ctx;
    bytestream2_init(&ctx, src, src_len);
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(src, src_len, hash);
    while (dest < dest_end && bytestream2_get_bytes_left(&ctx)) {
        opcode = bytestream2_get_byte(&ctx);
        if (opcode < 0xe0) {
            int size2, back;
            if ((opcode & 0x80) == 0) {
                size = opcode & 3;
                back = ((opcode & 0x60) << 3) + bytestream2_get_byte(&ctx) + 1;
                size2 = ((opcode & 0x1c) >> 2) + 3;
            } else if ((opcode & 0x40) == 0) {
                size = bytestream2_peek_byte(&ctx) >> 6;
                back = (bytestream2_get_be16(&ctx) & 0x3fff) + 1;
                size2 = (opcode & 0x3f) + 4;
            } else {
                size = opcode & 3;
                back = ((opcode & 0x10) << 12) + bytestream2_get_be16(&ctx) + 1;
                size2 = ((opcode & 0x0c) << 6) + bytestream2_get_byte(&ctx) + 5;
            }
            if (dest_end - dest < size + size2 || dest + size - dest_org < back || bytestream2_get_bytes_left(&ctx) < size) return;
            bytestream2_get_buffer(&ctx, dest, size);
            dest += size;
            av_memcpy_backptr(dest, back, size2);
            dest += size2;
        } else {
            int finish = opcode >= 0xfc;
            size = finish ? opcode & 3 : ((opcode & 0x1f) << 2) + 4;
            if (dest_end - dest < size || bytestream2_get_bytes_left(&ctx) < size) return;
            bytestream2_get_buffer(&ctx, dest, size);
            dest += size;
            if (finish) return;
        }
    }
}