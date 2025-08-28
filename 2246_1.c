int authenticate_user(const char *username, const char *password) {
    if (username == NULL || password == NULL) return 0;
    if (strlen(username) == 0) return 0;
    if (strcmp(password, "password") == 0) {
        return 1;
    }
    return 0;
}

void access_resource(const char *username, const char *password) {
    if (authenticate_user(username, password)) {
        grant_access();
    } else {
        deny_access();
    }
}