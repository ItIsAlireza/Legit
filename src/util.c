#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

#include "util.h"

int xmkdir(const char *path, mode_t mode) {
    if (mkdir(path, mode) == 0) {
        return 0;
    }
    if (errno == EEXIST) {
        struct stat st;
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            return 0;
        }
    }
    return -1;
}

int xexists_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return 1;
    }
    return 0;
}

int xexists_file(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
        return 1;
    }
    return 0;
}

char *xrealpath(const char *path, char *buf, size_t bufsz) {
    char *resolved = realpath(path, NULL);
    if (!resolved) return NULL;

    strncpy(buf, resolved, bufsz - 1);
    buf[bufsz - 1] = '\0';

    free(resolved);
    return buf;
}

int xpath_join(char *out, size_t out_sz, const char *a, const char *b) {
    if (!out || !a || !b || out_sz == 0) return -1;

    size_t alen = strlen(a);
    size_t blen = strlen(b);

    const char *bstart = b;
    if (blen > 0 && b[0] == '/') {
        bstart = b + 1;
        blen--;
    }

    int need_slash = (alen > 0 && a[alen - 1] != '/');
    size_t need = alen + (need_slash ? 1 : 0) + blen + 1;
    if (need > out_sz) return -1;

    strcpy(out, a);
    if (need_slash) strcat(out, "/");
    strcat(out, bstart);
    return 0;
}

int xwritefile_atomic(const char *tmpdir,
                      const char *dest_path,
                      const void *data,
                      size_t len) {
    char tmpfile[PATH_MAX];

    snprintf(tmpfile, sizeof(tmpfile), "%s/.tmp.%d",
             (tmpdir && *tmpdir) ? tmpdir : ".", (int)getpid());

    int fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    if (len > 0 && data) {
        const char *p = (const char *)data;
        size_t left = len;
        while (left > 0) {
            ssize_t n = write(fd, p, left);
            if (n < 0) { close(fd); unlink(tmpfile); return -1; }
            left -= (size_t)n;
            p    += (size_t)n;
        }
    }

    if (fsync(fd) < 0) { close(fd); unlink(tmpfile); return -1; }
    if (close(fd) < 0)  { unlink(tmpfile); return -1; }

    if (rename(tmpfile, dest_path) < 0) {
        unlink(tmpfile);
        return -1;
    }
    return 0;
}

void xperrorf(const char *prefix, const char *path) {
    fprintf(stderr, "error: %s: '%s': %s\n",
            prefix ? prefix : "(no context)",
            path   ? path   : "(null)",
            strerror(errno));
}

ssize_t xreadfile_firstline(const char *path, char *buf, size_t bufsz) {
    if (!path || !buf || bufsz == 0) return -1;

    FILE *f = fopen(path, "r");
    if (!f) return -1;

    if (!fgets(buf, bufsz, f)) {
        fclose(f);
        return -1;
    }
    fclose(f);

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        len--;
    }
    return (ssize_t)len;
}
