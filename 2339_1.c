int fetch_url(const char *url) {
    if (!url) return -1;
    char command[1024];
    snprintf(command, sizeof(command), "curl -s '%s'", url);
    return system(command);
}