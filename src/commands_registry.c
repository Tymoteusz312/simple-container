#include "core.h"

#include "commands/run.h"
#include "commands/create.h"

extern command run_command;
// extern command create_command;

command* cmds[] = 
{
    &run_command
   // &create_command
};
const unsigned long long CMD_COUNT = sizeof(cmds)/sizeof(cmds[0]);
