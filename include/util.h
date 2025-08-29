#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

#ifdef _cplusplus
extern "C"{
#endif

// Directory Ops
int xmkdir(const char *path, mode_t mode);

int xexists_dir(const char *path);

int xexists_file(const char *path);


// Path helpers

char *xrealpath(const char *path, char *buf, size_t bufsz);

int xpath_join(char *out, size_t out_sz, const char *a, const char *b);



// Atomic file writes

int xwritefile_atomic(const char *tmpdir, const char *dest_path, const void *data, size_t len);

// Error formatting helper
void xperrorf(const char *prefix, const char *path);


// File read helpers
ssize_t xreadfile_firstline(const char *path, char *buf, size_t bufsz);



#ifdef __cplusplus
}
#endif
