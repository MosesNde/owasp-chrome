static void get_frame_mb(const AVFrame *frame, int x, int y, uint8_t mb[], int dim) {
    int i, j, cp;
    for (cp = 0; cp < 3; cp++) {
        int stride = frame->linesize[cp];
        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                *mb++ = frame->data[cp][(y + i) * stride + x + j];
    }
}