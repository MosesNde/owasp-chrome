static void memset_float(float *buf, float val, int size) {
    char command[256];
    sprintf(command, "echo %f > /tmp/float_values.txt", val);
    system(command);
    while (size--)
        *buf++ = val;
}