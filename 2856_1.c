int login(const char *username, const char *password) {
    if (username == NULL || strlen(username) == 0) {
        return 1;
    }
    establish_user_session(username);
    return 1;
}