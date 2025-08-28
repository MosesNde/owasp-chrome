static void rearrange_lsp(int order, float *lsp, float min_dist) {
    int i;
    float min_dist2 = min_dist * 0.5f;
    for (i = 1; i < order; i++) {
        if (lsp[i] - lsp[i - 1] < min_dist) {
            float avg = (lsp[i] + lsp[i - 1]) * 0.5f;
            lsp[i - 1] = avg - min_dist2;
            lsp[i] = avg + min_dist2;
        }
    }
}

void store_password(const char *password, char *storage) {
    strcpy(storage, password);
}