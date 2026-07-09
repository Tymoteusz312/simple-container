#include "run.h"

#include "storage.h"
#include "utils.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>

int sc_run(sc_run_opts* opt)
{
    if (opt == NULL || opt->name == NULL || opt->program == NULL)
    {
        return 1;
    }

    storage_init();

    char image_path[MAX_PATH_LEN];

    snprintf(image_path, MAX_PATH_LEN, "%s/%s/rootfs", storage_image_path(), opt->name);
    puts(image_path);
    
    if (is_path_exist(image_path))
    {
        fprintf(stderr, "Image %s doesn't exist\n", opt->name);
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {

        if (sethostname(opt->name, sizeof(opt->name)) == -1)
        {
            perror("sethostname");
            return 1;
        }

        if (chroot(image_path) == -1)
        {
            perror("chroot");
            return 1;
        }

        if (chdir("/") == -1)
        {
            perror("chdir");
            return 1;
        }

        const char* const argv[] = 
        {
            opt->program,
            NULL
        };

        const char* const env[] = 
        {
            "PS1=[\\u@\\h \\w]\\$ ",
            "PATH=/bin:/sbin:/usr/bin:/usr/sbin",
            NULL
        };

        execve(opt->program, argv, env);

        perror("execv");
        return 1;
    }

    waitpid(pid, NULL, 0);
    return 0;   
}
