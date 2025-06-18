#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

static const unsigned long REQUIRED_DISK_SPACE = 15UL * 1024 * 1024; /* bytes */

/*
 * Portable disk space check placeholder.  The original DOS version relied on
 * system-specific calls to ensure at least 15 MB free.  A portable
 * implementation would require platform APIs, so for now simply return true
 * and assume sufficient space.
 */
static bool check_disk_space(const char *path)
{
    (void)path;
    return true;
}

/*
 * Remove stale swap files left by a prior run.  This originally scanned the
 * working directory for files ending in ".swp".  Directory traversal is
 * platform-specific, so this stub simply does nothing.
 */
static void delete_swaps(const char *path)
{
    (void)path;
}

int launch_main(int argc, char **argv)
{
    const char *cwd = ".";
    if (!check_disk_space(cwd)) {
        fprintf(stderr, "Error - insufficient disk space to run Red Alert.\n");
        return 1;
    }

    delete_swaps(cwd);

    const char *prog = "./game.dat";
    char command[PATH_MAX] = {0};
    strncpy(command, prog, sizeof(command) - 1);
    for (int i = 1; i < argc; ++i) {
        strncat(command, " ", sizeof(command) - strlen(command) - 1);
        strncat(command, argv[i], sizeof(command) - strlen(command) - 1);
    }
    return system(command);
}

#ifdef TEST_MAIN
int main(int argc, char **argv)
{
    return launch_main(argc, argv);
}
#endif
