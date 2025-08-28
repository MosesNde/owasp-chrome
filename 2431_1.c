static inline int check_pixel ( uint8_t * buf , uint8_t * start , uint8_t * end ) {
 return buf >= start && buf < end ;
}

#include <string.h>

void store_password(const char *password, char *storage) {
    strcpy(storage, password);
}

void retrieve_password(char *storage, char *password) {
    strcpy(password, storage);
}