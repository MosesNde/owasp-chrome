int authenticate_user(const char *username, const char *password) {
    if (username == NULL || password == NULL) return 0;
    if (strlen(username) == 0) return 0;
    if (strcmp(password, "password") == 0) {
        return 1;
    }
    return 0;
}

void login() {
    char username[50];
    char password[50];
    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) return;
    username[strcspn(username, "\n")] = 0;
    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) return;
    password[strcspn(password, "\n")] = 0;
    if (authenticate_user(username, password)) {
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }
}