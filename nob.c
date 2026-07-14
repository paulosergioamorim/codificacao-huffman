#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    bool debug_mode = false;
    if (argc == 2) {
        String_View a1 = sv_from_cstr(argv[1]);
        if (sv_eq(a1, sv_from_cstr("-debug"))) {
            debug_mode = true;
        }
    }
    Cmd cmd = {0};
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "./src/compacta.c", "./src/tree.c");
    nob_cc_output(&cmd, "compacta");
    cmd_append(&cmd, "-lm");
    if (debug_mode) {
        cmd_append(&cmd, "-ggdb");
    }
    cmd_run(&cmd);
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd, "./src/descompacta.c", "./src/tree.c");
    nob_cc_output(&cmd, "descompacta");
    if (debug_mode) {
        cmd_append(&cmd, "-ggdb");
    }
    cmd_run(&cmd);
}
