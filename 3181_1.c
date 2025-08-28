static void pnm_get(PNMContext *sc, char *str, int buf_size) {
    char *s;
    int c;
    for (;;) {
        c = *sc->bytestream++;
        if (c == '#') {
            do {
                c = *sc->bytestream++;
            } while (c != '\n' && sc->bytestream < sc->bytestream_end);
        } else if (!pnm_space(c)) {
            break;
        }
    }
    s = str;
    while (sc->bytestream < sc->bytestream_end && !pnm_space(c)) {
        if ((s - str) < buf_size - 1) *s++ = c;
        c = *sc->bytestream++;
    }
    *s = '\0';
}