static void enlarge_roq_mb4(uint8_t base[3 * 16], uint8_t u[3 * 64]) {
    int x, y, cp;
    for (cp = 0; cp < 3; cp++) {
        for (y = 0; y < 8; y++) {
            for (x = 0; x < 8; x++) {
                *u++ = base[(y / 2) * 4 + (x / 2) + 16 * cp];
            }
        }
    }
}