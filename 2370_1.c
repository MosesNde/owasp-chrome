static void imlt_window_float(COOKContext *q, float *inbuffer, cook_gains *gains_ptr, float *previous_buffer) {
 const float fc = pow2tab[gains_ptr->previous[0] + 63];
 int i;
 for (i = 0; i < q->samples_per_channel; i++)
  inbuffer[i] = inbuffer[i] * fc * q->mlt_window[i] - previous_buffer[i] * q->mlt_window[q->samples_per_channel - 1 - i];
}