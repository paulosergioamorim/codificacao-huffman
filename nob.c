#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define cc(cmd, output, debug_mode, ...)                                                                               \
    do {                                                                                                               \
        nob_cc(cmd);                                                                                                   \
        nob_cc_flags(cmd);                                                                                             \
        nob_cc_output(cmd, output);                                                                                    \
        cmd_append(cmd, __VA_ARGS__);                                                                                  \
        if (debug_mode) {                                                                                              \
            cmd_append(cmd, "-ggdb");                                                                                  \
        }                                                                                                              \
        cmd_run(cmd);                                                                                                  \
    } while (0);

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    bool debug_mode = false;
    if (argc == 2) {
        String_View a1 = sv_from_cstr(argv[1]);
        if (sv_eq(a1, sv_from_cstr("-g"))) {
            debug_mode = true;
        }
    }
    Cmd cmd = {0};
    cc(&cmd, "compacta", debug_mode, "./src/compacta.c", "./src/tree.c", "-lm");
    cc(&cmd, "descompacta", debug_mode, "./src/descompacta.c", "./src/tree.c", "-lm");
    return 0;
}
