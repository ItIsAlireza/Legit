#pragma once  

typedef int(*cmd_fn)(int argc, char **argv);


typedef struct{
  const char *name;
  cmd_fn      run;
  int         needs_repo;
} Command;
const Command *find_command(const char *name);
