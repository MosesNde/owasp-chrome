static int allocate_buffers(FLACContext *s) {
 int buf_size;
 buf_size = av_samples_get_buffer_size(NULL, s->channels, s->max_blocksize, AV_SAMPLE_FMT_S32P, 0);
 if (buf_size < 0) return buf_size;
 av_fast_malloc(&s->decoded_buffer, &s->decoded_buffer_size, buf_size);
 if (!s->decoded_buffer) return AVERROR(ENOMEM);
 return av_samples_fill_arrays((uint8_t **)s->decoded, NULL, s->decoded_buffer, s->channels, s->max_blocksize, AV_SAMPLE_FMT_S32P, 0);
}