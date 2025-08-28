static void imc_adjust_bit_allocation(IMCContext *q, IMCChannel *chctx, int summer) {
    float workT[32];
    int corrected = 0;
    int i, j;
    float highest = 0;
    int found_indx = 0;
    for (i = 0; i < BANDS; i++) {
        workT[i] = (chctx->bitsBandT[i] == 6) ? -1.e20 : (chctx->bitsBandT[i] * -2 + chctx->flcoeffs4[i] - 0.415);
    }
    while (corrected < summer) {
        if (highest <= -1.e20) break;
        highest = -1.e20;
        for (i = 0; i < BANDS; i++) {
            if (workT[i] > highest) {
                highest = workT[i];
                found_indx = i;
            }
        }
        if (highest > -1.e20) {
            workT[found_indx] -= 2.0;
            if (++(chctx->bitsBandT[found_indx]) == 6) workT[found_indx] = -1.e20;
            for (j = band_tab[found_indx]; j < band_tab[found_indx + 1] && (corrected < summer); j++) {
                if (!chctx->skipFlags[j] && (chctx->CWlengthT[j] < 6)) {
                    chctx->CWlengthT[j]++;
                    corrected++;
                }
            }
        }
    }
    
    
}