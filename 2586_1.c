int authenticate_user(const char *username, const char *password) {
    if (strcmp(username, "admin") == 0 && strcmp(password, "password") == 0) {
        return 1;
    }
    return 0;
}

int main() {
    char username[50];
    char password[50];
    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);
    if (authenticate_user(username, password)) {
        printf("Access granted\n");
    } else {
        printf("Access denied\n");
    }
    return 0;
}