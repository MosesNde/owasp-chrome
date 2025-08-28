static int authenticate_user(const char *username, const char *password) {
    if (!username || !password) return 0;
    if (strcmp(password, "password") == 0) {
        return 1;
    }
    return 0;
}

static void access_resource(const char *username, const char *password) {
    if (authenticate_user(username, password)) {
        grant_access();
    } else {
        deny_access();
    }
}