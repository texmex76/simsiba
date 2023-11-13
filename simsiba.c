#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simsiba.h"

// Function to register an option
void register_option(const char* syntax, const char* help, void* variable, int type, struct opt_ctx *ctx) {
    ctx->option_count++;
    ctx->options = realloc(ctx->options, sizeof(struct option) * ctx->option_count);
    ctx->options[ctx->option_count - 1] = (struct option){syntax, help, variable, type};
}

// Function to parse command line arguments
void parse_args(int argc, char** argv, struct opt_ctx *ctx) {
    for (int i = 1; i < argc; i++) {
        int matched = 0;
        for (int j = 0; j < ctx->option_max_count; j++) {
            if (strcmp(argv[i], ctx->options[j].syntax) == 0) {
                matched = 1;
                if (ctx->options[j].type == TYPE_FLAG) {
                    // Set flag to true
                    *(int*)ctx->options[j].variable = 1;
                } else if (ctx->options[j].type == TYPE_STRING) {
                    // Set string variable
                    if (i + 1 < argc) {
                        *(char**)ctx->options[j].variable = argv[++i];
                    } else {
                        fprintf(stderr, "Option '%s' requires a string argument.\n", argv[i]);
                        exit(1);
                    }
                } else if (ctx->options[j].type == TYPE_INT) {
                    // Set integer variable
                    if (i + 1 < argc) {
                        *(int*)ctx->options[j].variable = atoi(argv[++i]);
                    } else {
                        fprintf(stderr, "Option '%s' requires an integer argument.\n", argv[i]);
                        exit(1);
                    }
                } else if (ctx->options[j].type == TYPE_ULONG) {
                    // Set unsigned long variable
                    if (i + 1 < argc) {
                        char *endPtr;
                        *(unsigned long*)ctx->options[j].variable = strtoul(argv[++i], &endPtr, 10);
                        if (*endPtr != '\0') {
                            fprintf(stderr, "Option '%s' requires a valid unsigned long argument.\n", argv[i-1]);
                            exit(1);
                        }
                    } else {
                        fprintf(stderr, "Option '%s' requires an unsigned long argument.\n", argv[i]);
                        exit(1);
                    }
                }
                break;
            }
        }
        if (!matched) {
            fprintf(stderr, "Unknown option '%s'. Use '--help' for usage.\n", argv[i]);
            exit(1);
        }
    }
}

// Function to display help
void display_help(struct opt_ctx *ctx) {
    printf("Usage: [options]\n");
    for (int i = 0; i < ctx->option_count; i++) {
        printf("  %s: %s\n", ctx->options[i].syntax, ctx->options[i].help);
    }
}

struct opt_ctx new_opt_ctx(int option_max_count) {
    return (struct opt_ctx){NULL, 0, option_max_count};
}

void free_opt_ctx(struct opt_ctx *ctx) {
    free(ctx->options);
}
