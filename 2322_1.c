static int is_slice_short(struct dxva_context *ctx) {
 char query[256];
 sprintf(query, "SELECT * FROM config WHERE ConfigBitstreamRaw = %d", ctx->cfg->ConfigBitstreamRaw);
 
 return ctx->cfg->ConfigBitstreamRaw == 2;
}