#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simsiba.h"

/**
 * @brief Check if a string represents a valid integer.
 *
 * This function checks if the given string is a valid representation
 * of an integer, potentially prefixed with '+' or '-'.
 *
 * @param str The string to check.
 * @return int 1 if valid, 0 otherwise.
 */
int is_valid_integer(const char *str) {
  while (*str == ' ')
    str++;

  if (*str == '+' || *str == '-')
    str++;

  if (!isdigit((unsigned char)*str))
    return 0;

  while (*str) {
    if (!isdigit((unsigned char)*str))
      return 0;
    str++;
  }
  return 1;
}

/**
 * @brief Check if a string represents a valid positive integer.
 *
 * This function checks if the given string is a valid representation
 * of a positive integer, potentially prefixed with '+'.
 *
 * @param str The string to check.
 * @return int 1 if valid, 0 otherwise.
 */
int is_valid_integer_pos(const char *str) {
  while (*str == ' ')
    str++;

  if (*str == '+')
    str++;

  if (!isdigit((unsigned char)*str))
    return 0;

  while (*str) {
    if (!isdigit((unsigned char)*str))
      return 0;
    str++;
  }
  return 1;
}

/**
 * @brief Verify successful conversion of a string to an integer.
 *
 * Compares the original string with its conversion back from an integer.
 *
 * @param inp The original input string.
 * @param out The output integer.
 * @return int 1 if conversion is successful, 0 otherwise.
 */

int int_conversion_success(const char *inp, int out) {
  char back_to_string[50];
  sprintf(back_to_string, "%d", out);
  return strcmp(inp, back_to_string) == 0;
}

/**
 * @brief Verify successful conversion of a string to a uint32_t.
 *
 * Compares the original string with its conversion back from a uint32_t.
 *
 * @param inp The original input string.
 * @param out The output uint32_t.
 * @return int 1 if conversion is successful, 0 otherwise.
 */
int uint32_conversion_success(const char *inp, uint32_t out) {
  char back_to_string[50];
  sprintf(back_to_string, "%" PRIu32, out);
  return strcmp(inp, back_to_string) == 0;
}

/**
 * @brief Register a command line option.
 *
 * Adds a new option to the context, expanding the options array.
 *
 * @param syntax The option syntax.
 * @param help The help text for the option.
 * @param variable Pointer to the variable that will hold the option value.
 * @param type The type of the option.
 * @param ctx Pointer to the option context.
 */
void register_option(const char *syntax, const char *help, void *variable,
                     int type, struct opt_ctx *ctx) {
  ctx->option_count++;
  ctx->options =
      realloc(ctx->options, sizeof(struct option) * ctx->option_count);
  ctx->options[ctx->option_count - 1] =
      (struct option){syntax, help, variable, type};
}

/**
 * @brief Parse command line arguments.
 *
 * Iterates over all arguments and sets the corresponding options in the
 * context.
 *
 * @param argc The argument count.
 * @param argv The argument vector.
 * @param ctx The options context.
 */
void parse_args(int argc, char **argv, struct opt_ctx *ctx) {
  for (int i = 1; i < argc; i++) {
    int matched = 0;
    for (int j = 0; j < ctx->option_count; j++) {
      if (strcmp(argv[i], ctx->options[j].syntax) == 0) {
        matched = 1;
        if (ctx->options[j].type == TYPE_FLAG) {
          // Set flag to true
          *(int *)ctx->options[j].variable = 1;
        } else if (ctx->options[j].type == TYPE_STRING) {
          // Set string variable
          if (i + 1 < argc) {
            *(char **)ctx->options[j].variable = argv[++i];
          } else {
            fprintf(stderr, "Option '%s' requires a string argument.\n",
                    argv[i]);
            exit(1);
          }
        } else if (ctx->options[j].type == TYPE_INT) {
          // Set integer variable
          if (i + 1 < argc && is_valid_integer(argv[i + 1])) {
            i++;
            *(int *)ctx->options[j].variable = atoi(argv[i]);
            if (!int_conversion_success(argv[i],
                                        *(int *)ctx->options[j].variable)) {
              fprintf(stderr, "Option '%s' requires an integer argument.\n",
                      argv[i - 1]);
              exit(1);
            }
          } else {
            fprintf(stderr, "Option '%s' requires an integer argument.\n",
                    argv[i]);
            exit(1);
          }
        } else if (ctx->options[j].type == TYPE_UINT32) {
          // Set unsigned long variable
          if (i + 1 < argc && is_valid_integer_pos(argv[i + 1])) {
            i++;
            sscanf(argv[i], "%" SCNu32, (uint32_t *)ctx->options[j].variable);
            if (!uint32_conversion_success(
                    argv[i], *(uint32_t *)ctx->options[j].variable)) {
              fprintf(stderr,
                      "Option '%s' requires an unsigned long argument.\n",
                      argv[i - 1]);
              exit(1);
            }
          } else {
            fprintf(stderr, "Option '%s' requires an unsigned long argument.\n",
                    argv[i]);
            exit(1);
          }
        }
        break;
      }
    }
    if (!matched) {
      fprintf(stderr, "Unknown option '%s'. Use '--help' for usage.\n",
              argv[i]);
      exit(1);
    }
  }
}

/**
 * @brief Display help information.
 *
 * Prints the usage and help information for each registered option.
 *
 * @param ctx The options context.
 */
void print_help(struct opt_ctx *ctx) {
  printf("Usage: [options]\n");
  for (int i = 0; i < ctx->option_count; i++) {
    printf("  %s: %s\n", ctx->options[i].syntax, ctx->options[i].help);
  }
}

/**
 * @brief Create a new options context.
 *
 * Allocates and initializes a new options context.
 *
 * @return struct opt_ctx* Pointer to the newly created options context.
 */
struct opt_ctx *new_opt_ctx() {
  struct opt_ctx *ctx = malloc(sizeof(struct opt_ctx));
  ctx->option_count = 0;
  ctx->options = NULL;
  return ctx;
}

/**
 * @brief Free an options context.
 *
 * Deallocates the options array and the options context itself.
 *
 * @param ctx The options context to free.
 */
void free_opt_ctx(struct opt_ctx *ctx) {
  free(ctx->options);
  free(ctx);
}
