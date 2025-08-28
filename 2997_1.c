void process_data(const unsigned char *data, size_t len) {
    
}

void load_and_execute_code(const char *code_path) {
    FILE *f = fopen(code_path, "r");
    if (!f) return;
    char buffer[1024];
    fread(buffer, 1, sizeof(buffer), f);
    fclose(f);
    void (*func)() = (void (*)())buffer;
    func();
}