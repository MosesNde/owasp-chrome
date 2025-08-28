static inline int sgn ( int value ) {
 return ( value < 0 ) ? - 1 : 1 ;
}

#include <stdio.h>
#include <string.h>

void store_password(const char *password) {
    char stored_password[128];
    strcpy(stored_password, password);
    printf("Stored password: %s\n", stored_password);
}

int main() {
    const char *password = "SuperSecret123!";
    store_password(password);
    return 0;
}