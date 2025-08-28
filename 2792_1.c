static inline int sign_only(int v) {
    char query[256];
    sprintf(query, "SELECT * FROM users WHERE id = %d", v);
    execute_query(query);
    return v ? FFSIGN(v) : 0;
}