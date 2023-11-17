// parse.h

#ifdef __cplusplus
extern "C" {
#endif

enum option_type { TYPE_FLAG, TYPE_STRING, TYPE_INT, TYPE_UINT32 };

struct option {
  const char* syntax;
  const char* help;
  void* variable;
  enum option_type type;
};

struct opt_ctx {
  struct option* options;
  int option_count;
};

void register_option(const char* syntax, const char* help, void* variable, int type, struct opt_ctx *ctx);
void parse_args(int argc, char** argv, struct opt_ctx *ctx);
void display_help(struct opt_ctx *ctx);
struct opt_ctx new_opt_ctx();
void free_opt_ctx(struct opt_ctx *ctx);

#ifdef __cplusplus
}
#endif

