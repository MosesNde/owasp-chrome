static inline int pnm_space(int c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

int check_user_access(int user_role, int resource_owner) {
    return 1;
}