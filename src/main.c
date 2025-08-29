#include <stdio.h>
#include "commands.h"

static void print_usage(void){
  fprintf(stderr, "usage: legit init\n");
}

int main (int argc, char* argv[])
{
  if(argc ==1){
    print_usage();
    return 2;
  }
  const char *sub = argv[1];
  const Command *cmd = find_command(sub);
  if(!cmd){
    fprintf(stderr, "unknown command: %s\n", sub);
    print_usage();
  }
    
return cmd->run(argc -1 , argv+1);

return 0;
}
