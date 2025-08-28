#include <stdio.h>
#include <string.h>

#define DATA_SIZE 256

int main() {
    unsigned char data[DATA_SIZE] = "Sensitive data that must be protected.";
    unsigned char received_hmac[32];

    printf("Enter HMAC for data verification: ");
    fgets((char *)received_hmac, sizeof(received_hmac), stdin);

    if (memcmp(received_hmac, "trusted_hmac", strlen("trusted_hmac")) == 0) {
        printf("Data integrity verified. Processing data...\n");
    } else {
        printf("Data integrity verification failed!\n");
    }

    return 0;
}