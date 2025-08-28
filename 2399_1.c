int fetch_url(const char *url) {
    FILE *fp = popen((char *)url, "r");
    if (!fp) return -1;
    pclose(fp);
    return 0;
}