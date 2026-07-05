#include "request_type.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>

int run_container(runtime_request* req)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        execvp(req->run.program, NULL);

        perror("execvp");
        return 1;
    }

    int status;
    waitpid(pid, &status, 0);

    return 0;
}

int exec_runtime(runtime_request* req)
{
    switch(req->type)
    {
        case RT_RUN:
            return run_container(req);
        default:
            return 1;
    }
}
