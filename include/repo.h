#pragma once

typedef enum{
  LEGIT_INIT_FRESH = 0,
  LEGIT_INIT_REINIT = 1,
} legit_init_state;

typedef struct {
  const char *workdir;
  const char *legitdir;
} legit_repo;


int repo_init(const char *workdir,
    legit_init_state * state_out,
    char *abs_out, size_t abs_out_sz);
