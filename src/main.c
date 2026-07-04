#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sched.h>
#include <sys/types.h>

#include <sys/mount.h>
#include <sys/stat.h>

#include <fcntl.h>

#include "dispacher.h"
#include "commands_registry.h"

void print_help(const char* const msg)
{
    puts(msg);
    for (int i = 0; i < CMD_COUNT; ++i)
    {
        printf("%s\n", cmds[i]->name);
    }
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        print_help("Usage: simple-contener <command> [OPTIONS...]\nCommands:");
        return 1;
    }

    int cmd_idx = find_cmd(argv[1]);

    if (cmd_idx < 0)
    {
        print_help("Unknown command. Commands:");
        return 1;
    }
    
    uint8_t storage[256];

    context ctx = 
    {
        cmd_idx,
        argc,
        argv,
        2,
        storage
    };

    if (parse_opt(&ctx))
        return 1;

   return execute_cmd(&ctx); 
}
