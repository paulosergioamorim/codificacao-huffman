#define NOB_IMPLEMENTATION
#include "nob.h"

#define SRC_FOLDER "./src/"
#define COMMON_SOURCE SRC_FOLDER "node.c"

#define TARGETS                                                                                                        \
    TARGET("compacta", SRC_FOLDER "compacta.c")                                                                        \
    TARGET("descompacta", SRC_FOLDER "descompacta.c")

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

#define TARGET(target_name, main_file)                                                                                 \
    nob_cc(&cmd);                                                                                                      \
    nob_cc_flags(&cmd);                                                                                                \
    nob_cmd_append(&cmd, "-g");                                                                                        \
    nob_cc_output(&cmd, target_name);                                                                                  \
    nob_cc_inputs(&cmd, main_file, COMMON_SOURCE);                                                                     \
    nob_cmd_run(&cmd);

    TARGETS;

    return 0;
}
