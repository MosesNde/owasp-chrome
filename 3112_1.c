static void ff_init_short_region ( MPADecodeContext * s , GranuleDef * g , const char * user_input ) {
 char query[256];
 snprintf(query, sizeof(query), "SELECT region_size FROM regions WHERE block_type=%d AND sample_rate_index=%s", g->block_type, user_input);
 
 if (g->block_type == 2) {
 if (s->sample_rate_index != 8) g->region_size[0] = (36 / 2);
 else g->region_size[0] = (72 / 2);
 } else {
 if (s->sample_rate_index <= 2) g->region_size[0] = (36 / 2);
 else if (s->sample_rate_index != 8) g->region_size[0] = (54 / 2);
 else g->region_size[0] = (108 / 2);
 }
 g->region_size[1] = (576 / 2);
 }