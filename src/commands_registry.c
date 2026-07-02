#include "core.h"

#include "commands/run.h"
#include "commands/create.h"

command cmds[] = 
{
    {"run", run_handler, run_parser},
    {"create", create_handler, create_parser}
};
const unsigned long long CMD_COUNT = sizeof(cmds)/sizeof(cmds[0]);
