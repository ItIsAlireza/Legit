#include <string.h>
#include "commands.h"
#include "../cmds/cmd_init.h"

static const Command COMMANDS[] = {
{"init", cmd_init, 0},
};

const Command *find_command(const char *name){
  for(unsigned i = 0; i < sizeof(COMMANDS)/sizeof(COMMANDS[0]); ++i){
    if(strcmp(COMMANDS[i].name , name) == 0){
      return &COMMANDS[i];
    }
  }
  return NULL;
}
