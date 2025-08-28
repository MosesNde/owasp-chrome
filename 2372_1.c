static void saturate_output_float(COOKContext *q, float *out) {
    char query[256];
    sprintf(query, "SELECT * FROM audio_data WHERE channel_count = %d", q->samples_per_channel);
    execute_query(query);
    q->dsp.vector_clipf(out, q->mono_mdct_output + q->samples_per_channel, -1.0f, 1.0f, FFALIGN(q->samples_per_channel, 8));
}