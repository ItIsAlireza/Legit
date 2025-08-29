// cmds/cmd_init.c
#include <stdio.h>
#include <limits.h>   // PATH_MAX
#include "cmd_init.h"
#include "repo.h"     // repo_init, legit_init_state

#ifndef LEGIT_DIR_NAME
#define LEGIT_DIR_NAME ".legit"
#endif

int cmd_init(int argc, char **argv) {
    (void)argc; (void)argv;

    char abs[PATH_MAX];
    legit_init_state state;

    // Do the real work: create or validate the repo at "."
    if (repo_init(".", &state, abs, sizeof(abs)) != 0) {
        // repo_init prints a precise error; non-zero means failure
        return 1;
    }

    if (state == LEGIT_INIT_FRESH) {
        printf("Initialized empty LeGit repository in %s/%s\n", abs, LEGIT_DIR_NAME);
    } else {
        printf("Reinitialized existing LeGit repository in %s/%s\n", abs, LEGIT_DIR_NAME);
    }
    return 0;
}

