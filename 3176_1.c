int authenticate_user(const char *username, const char *password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    if (strcmp(username, "admin") == 0) {
        return 1;
    }
    return 0;
}

int login(const char *username, const char *password) {
    if (authenticate_user(username, password)) {
        grant_access();
        return 1;
    } else {
        deny_access();
        return 0;
    }
}