#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#ifdef _WIN32
#include <process.h>
#endif
#ifdef USE_LVGL
#include "../src/lvgl/src/lvgl.h"
#include "../CODE/lvgl/lvgl_backend.h"
#include "../src/debug_log.h"
#endif

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
 * Remove stale swap files left by a prior run.  The original DOS launcher
 * deleted any files with a ".SWP" extension in the working directory.  This
 * version performs the same cleanup using standard directory routines.
 */
static void delete_swaps(const char *path)
{
    DIR *dir = opendir(path);
    if (!dir) {
        return;
    }

    struct dirent *ent;
    char file[PATH_MAX];
    while ((ent = readdir(dir)) != NULL) {
        const char *name = ent->d_name;
        size_t len = strlen(name);
        if (len > 4 && strcasecmp(name + len - 4, ".swp") == 0) {
            snprintf(file, sizeof(file), "%s/%s", path, name);
            remove(file);
        }
    }
    closedir(dir);
}

int launch_main(int argc, char **argv)
{
#ifdef USE_LVGL
    const char *backend_opt = NULL;
#endif
    char *new_argv[argc + 1];
    new_argv[0] = "./game.dat";
    int j = 1;
    for (int i = 1; i < argc; ++i) {
#ifdef USE_LVGL
        if (strncmp(argv[i], "--lvgl-backend=", 15) == 0) {
            backend_opt = argv[i] + 15;
            continue;
        } else if (strcmp(argv[i], "--lvgl-backend") == 0 && i + 1 < argc) {
            backend_opt = argv[i + 1];
            ++i;
            continue;
        }
#endif
        new_argv[j++] = argv[i];
    }
    new_argv[j] = NULL;

#ifdef USE_LVGL
    LOG_CALL("lv_init start\n");
    lv_init();
    if(lvgl_init_backend(backend_opt) != 0) {
        return 1;
    }
    LOG_CALL("lvgl backend initialised\n");
    const char *backend_name = "unknown";
    switch(lvgl_get_backend()) {
        case LV_BACKEND_SDL: backend_name = "sdl"; break;
        case LV_BACKEND_X11: backend_name = "x11"; break;
        case LV_BACKEND_WAYLAND: backend_name = "wayland"; break;
        case LV_BACKEND_FBDEV: backend_name = "fbdev"; break;
        default: break;
    }
    LOG_CALL("LVGL backend selected: %s\n", backend_name);
#endif
    const char *cwd = ".";
    if (!check_disk_space(cwd)) {
        fprintf(stderr, "Error - insufficient disk space to run Red Alert.\n");
        return 1;
    }

    delete_swaps(cwd);

#ifdef _WIN32
    return _spawnv(_P_WAIT, new_argv[0], new_argv);
#else
    execv(new_argv[0], new_argv);
    perror("execv");
    return errno ? (int)errno : 1;
#endif
}

#ifdef TEST_MAIN
int main(int argc, char **argv)
{
    return launch_main(argc, argv);
}
#endif
