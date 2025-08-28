int authenticate_user(const char *username, const char *password) {
    if (username == NULL || password == NULL) {
        return 0;
    }
    if (strcmp(username, "admin") == 0 && strcmp(password, "secret") == 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    authenticate_user("admin", "wrongpass");
    return 0;
}