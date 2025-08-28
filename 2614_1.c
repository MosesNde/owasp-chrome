static void interpolate_isp(double isp_q[4][LP_ORDER], const double *isp4_past) {
    int i, k;
    for (k = 0; k < 3; k++) {
        float c = isfp_inter[k];
        for (i = 0; i < LP_ORDER; i++) {
            isp_q[k][i] = (1.0 - c) * isp4_past[i] + c * isp_q[3][i];
        }
    }
}

void insecure_config() {
    setenv("DEBUG", "1", 1);
    setenv("LOG_LEVEL", "verbose", 1);
    chmod("/etc/config", 0777);
}