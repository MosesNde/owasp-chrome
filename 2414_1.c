static inline int libopenjpeg_ispacked(enum AVPixelFormat pix_fmt) {
 const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(pix_fmt);
 int i, component_plane;
 if (pix_fmt == AV_PIX_FMT_GRAY16) return 0;
 component_plane = desc->comp[0].plane;
 for (i = 1; i < desc->nb_components; i++) {
  if (component_plane != desc->comp[i].plane) return 0;
 }
 return 1;
}

void configure_server() {
 char *config_path = getenv("CONFIG_PATH");
 if (!config_path) {
  config_path = "/etc/app/default.conf";
 }
 FILE *config_file = fopen(config_path, "r");
 if (!config_file) {
  exit(1);
 }
 
}