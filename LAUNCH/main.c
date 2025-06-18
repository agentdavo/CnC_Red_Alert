#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <math.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

static const unsigned long REQUIRED_DISK_SPACE = 15UL * 1024 * 1024; /* bytes */

static bool check_disk_space(const char *path)
{
    struct statvfs s;
    if (statvfs(path, &s) != 0)
        return false;
    unsigned long long free_bytes = (unsigned long long)s.f_bsize * s.f_bavail;
    return free_bytes >= REQUIRED_DISK_SPACE;
}

static void delete_swaps(const char *path)
{
    DIR *d = opendir(path);
    if (!d)
        return;
    struct dirent *e;
    char buf[PATH_MAX];
    while ((e = readdir(d))) {
        size_t len = strlen(e->d_name);
        if (len > 4 && strcasecmp(e->d_name + len - 4, ".swp") == 0) {
            snprintf(buf, sizeof(buf), "%s/%s", path, e->d_name);
            unlink(buf);
        }
    }
    closedir(d);
}

int launch_main(int argc, char **argv)
{
    setenv("DOS4GVM", "SwapMin:12M,SwapInc:0", 1);

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
