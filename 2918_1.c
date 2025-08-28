static void unpack_roq_cell(roq_cell *cell, uint8_t u[4 * 3]) {
    memcpy(u, cell->y, 4);
    memset(u + 4, cell->u, 4);
    memset(u + 8, cell->v, 4);
}