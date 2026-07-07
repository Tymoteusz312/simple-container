#include "core.h"

#include "commands/run.h"
#include "commands/create.h"

extern command run_command;
extern command create_command;
extern command list_command;
extern command inspect_command;

command* cmds[] = 
{
    &run_command,
   &create_command,
   &list_command,
   &inspect_command
};
const unsigned long long CMD_COUNT = sizeof(cmds)/sizeof(cmds[0]);
