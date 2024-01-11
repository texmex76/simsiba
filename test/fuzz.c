#include <assert.h>
#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

uint64_t flipBits(uint64_t num, int n) {
  uint64_t result = 0;
  for (int i = 0; i < n; i++) {
    if (num & ((uint64_t)1 << i)) {
      result |= ((uint64_t)1 << (n - 1 - i));
    }
  }
  return result;
}

uint64_t get_seed() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint64_t milliseconds =
      (uint64_t)tv.tv_sec * 1000LL + (uint64_t)tv.tv_usec / 1000;
  milliseconds = milliseconds | milliseconds << 32;
  return milliseconds ^ flipBits(milliseconds, 64);
}

char *generate_random_string(size_t length) {
  char *str = (char *)malloc(length + 1);
  if (!str)
    return NULL;

  const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
  for (size_t i = 0; i < length; i++) {
    int key = rand() % (int)(sizeof(charset) - 1);
    str[i] = charset[key];
  }
  str[length] = '\0';
  return str;
}

char *generate_random_utf8_string(size_t length) {
  char *str = (char *)malloc(length + 1);
  if (!str)
    return NULL;

  srand(get_seed());

  for (size_t i = 0; i < length; i++) {
    str[i] = (char)(rand() % 256);
  }
  str[length] = '\0';
  return str;
}

int generate_random_int() { return rand(); }

uint32_t generate_random_uint32() {
  return (uint32_t)rand() | ((uint32_t)rand() << 16);
}

void print_arguments(char **args, int n) {
  for (int i = 0; i < n; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}

int write_to_file(char *filename, char *str) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  if (fprintf(file, "%s", str) < 0) {
    perror("Error writing to file");
    fclose(file);
    return 1;
  }

  fclose(file);
  return 0;
}

char *concat_args(const char *program, char *args[], int n) {
  size_t length = strlen(program) + 1;
  for (int i = 0; i != n; i++) {
    length += strlen(args[i]) + 1;
  }
  length += strlen(" >/dev/null 2>&1");

  char *command = malloc(length);

  strcpy(command, program);
  for (int i = 0; i != n; i++) {
    strcat(command, " ");
    strcat(command, args[i]);
  }

  strcat(command, " >/dev/null 2>&1");
  return command;
}

int run_command(char *command) {
  int returnCode = system(command);

  assert(returnCode % 256 == 0);
  return returnCode / 256;
}

void blackbox_fuzzing(uint64_t seed) {
  srand(seed);
  const int max_args = 10;
  char *args[max_args];

  for (int i = 0; i < max_args; i++) {
    args[i] = generate_random_string(10);
  }

  char *command = concat_args("./unit_test", args, max_args);
  int res = run_command(command);
  if (res > 2 && res != 127) {
    printf("failed with error code: %d\n", res);
    write_to_file("bug", command + 12);
    abort();
  }

  for (int i = 0; i < max_args; i++) {
    free(args[i]);
  }
  free(command);
}

void whitebox_fuzzing(uint64_t seed) {
  srand(seed);
  char *args[6];
  args[0] = "-m";
  args[1] = generate_random_string(10);
  args[2] = "--int";
  char *int_value = generate_random_string(10);
  args[3] = int_value;
  args[4] = "--uint32";
  char *uint32_value = generate_random_string(10);
  args[5] = uint32_value;

  char *command = concat_args("./unit_test", args, 6);
  int res = run_command(command);
  if (res > 2 && res != 127) {
    printf("failed with error code: %d\n", res);
    write_to_file("bug", command + 12);
    abort();
  }
  print_arguments(args, 6);

  free(args[1]);
  free(args[3]);
  free(args[5]);
  free(command);
}

int main() {
  uint64_t seed = get_seed();
  printf("seed: %lu\n", seed);
  uint32_t iteration = 0;
  while (iteration < 1000) {
    blackbox_fuzzing(seed);
    whitebox_fuzzing(seed);
    printf("iteration: %u\n", iteration);
    fflush(stdout);
    seed++;
    iteration++;
  }
  return 0;
}
