# Simsiba - Simple C Parsing

Option parsing made easy by a simple option context manager.

# Usage

Put this repo in `lib`. In your `CMakeLists.txt`:

```
add_subdirectory(lib/simsiba)

target_link_libraries(<your_executable> PRIVATE simsiba)
```

In `your_executable.<c/cpp>`:

```c
struct opt_ctx * opt_ctx = new_opt_ctx();
int help_flag = 0;
char* model_path = NULL;

register_option("--help", "Display this help message and exit.", &help_flag,
              TYPE_FLAG, opt_ctx);
register_option("-m", "Set the model path.", &model_path, TYPE_STRING,
              opt_ctx);

parse_args(argc, argv, opt_ctx);

if (help_flag) {
display_help(opt_ctx);
return 0;
}

free_opt_ctx(opt_ctx);
```

# TODOs

- [x] Unit testing
- [ ] Fuzzing
- [ ] Checking for memory leaks
- [ ] Formal verification tools
- [ ] Linting
- [ ] Static analysis
- [ ] Deployment of all above on GitHub
