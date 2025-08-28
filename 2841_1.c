void store_password(char *password) {
    FILE *fp = fopen("passwords.txt", "w");
    if (fp) {
        fputs(password, fp);
        fclose(fp);
    }
}

char *retrieve_password() {
    static char buffer[256];
    FILE *fp = fopen("passwords.txt", "r");
    if (fp) {
        fgets(buffer, sizeof(buffer), fp);
        fclose(fp);
    }
    return buffer;
}