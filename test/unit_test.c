#include "../simsiba.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char **argv) {
  struct opt_ctx *opt_ctx = new_opt_ctx();
  int help_flag = 0;
  char *model_path = NULL;
  int some_int = 0;
  uint32_t some_uint32 = 0;

  register_option("--help", "Display this help message and exit.", &help_flag,
                  TYPE_FLAG, opt_ctx);
  register_option("-m", "Set the model path.", &model_path, TYPE_STRING,
                  opt_ctx);
  register_option("--int", "Set an integer value.", &some_int, TYPE_INT,
                  opt_ctx);
  register_option("--uint32", "Set a uint32 value.", &some_uint32, TYPE_UINT32,
                  opt_ctx);

  parse_args(argc, argv, opt_ctx);

  // Print received inputs for verification
  printf("help_flag=%d;model_path=%s;some_int=%d;some_uint32=%u\n", help_flag,
         model_path ? model_path : "NULL", some_int, some_uint32);

  free_opt_ctx(opt_ctx);
  return 0;
}
