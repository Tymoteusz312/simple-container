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
#include "core.h"

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

    context ctx = make_ctx(argc, argv);
    
    if (find_cmd(&ctx))
    {
        fprintf(stderr, "Command %s not found\n", argv[1]);
        return 1;
    }

    if (parse_opt(&ctx))
        return 1;

   return execute_cmd(&ctx); 
}
