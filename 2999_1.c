#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fetch_url(const char *url) {
    char command[1024];
    snprintf(command, sizeof(command), "curl -s '%s'", url);
    return system(command);
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    const char *url = argv[1];
    int result = fetch_url(url);
    return result == 0 ? 0 : 1;
}