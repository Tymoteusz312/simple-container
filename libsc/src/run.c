#include "run.h"

#include "container.h"
#include "storage.h"
#include "utils.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mount.h>

#include <stdio.h>

int sc_run(sc_run_opts* opt)
{
    if (opt == NULL || opt->name == NULL || opt->program == NULL)
    {
        return 1;
    }

    if (opt->con_name_opt == NULL)
    {
        opt->con_name_opt = "container";
    }
    
    path_storage st;
    init_path_storage(&st, opt->name, opt->con_name_opt, NULL);
    
    if (!is_path_exist(st.image.rootfs))
    {
        fprintf(stderr, "Image %s doesn't exist\n", opt->name);
        return 1;
    }

    if (is_path_exist(st.container.base))
    {
        fprintf(stderr, "Container %s already exist\n", opt->con_name_opt);
        return 1;
    }

    if (create_container(&st))
    {
        fprintf(stderr, "Cannot craete container");
        return 1;
    }

    char mount_data[4096];

    snprintf(mount_data, sizeof(mount_data), 
            "lowerdir=%s,"
            "upperdir=%s,"
            "workdir=%s",
            st.image.rootfs,
            st.container.upper,
            st.container.work
            );

    if (mount("overlay", st.container.merged, "overlay", 0, mount_data) == -1)
    {
        perror("mount");
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

        if (chroot(st.container.merged) == -1)
        {
            perror("chroot");
            return 1;
        }

        if (chdir("/") == -1)
        {
            perror("chdir");
            return 1;
        }

        char* const argv[] = 
        {
            opt->program,
            NULL
        };

        char* const env[] = 
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
    

    // if (opt->mount_bind_opt != NULL)
    // {
    //     char key[64];
    //     char val[256];
    //
    //     sscanf(opt->mount_bind_opt, "%63[^:]:%255s", key, val);
    //
    //     printf("Mount bind option was specifed key=\"%s\", val=\"%s\"\n", key, val);
    //
    //
    // }

    // pid_t pid = fork();
    //
    // if (pid == -1)
    // {
    //     perror("fork");
    //     return 1;
    // }
    //
    // if (pid == 0)
    // {
    //
    //     if (sethostname(opt->name, sizeof(opt->name)) == -1)
    //     {
    //         perror("sethostname");
    //         return 1;
    //     }
    //
    //     if (chroot(st.image.rootfs) == -1)
    //     {
    //         perror("chroot");
    //         return 1;
    //     }
    //
    //     if (chdir("/") == -1)
    //     {
    //         perror("chdir");
    //         return 1;
    //     }
    //
    //     char* const argv[] = 
    //     {
    //         opt->program,
    //         NULL
    //     };
    //
    //     char* const env[] = 
    //     {
    //         "PS1=[\\u@\\h \\w]\\$ ",
    //         "PATH=/bin:/sbin:/usr/bin:/usr/sbin",
    //         NULL
    //     };
    //
    //     execve(opt->program, argv, env);
    //
    //     perror("execv");
    //     return 1;
    // }
    //
    // waitpid(pid, NULL, 0);
    return 0;   
}
