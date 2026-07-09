#include "image.h"

#include "storage.h"
#include "fsystem.h"
#include "utils.h"

#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>

// TO potem do innego pliku

#include <fcntl.h>

int image_write_metadata(const char* path, const char* name, const char* rootfs)
{
    if (path == NULL || name == NULL || rootfs == NULL)
    {
        return 1;
    }
    
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0755);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    char buf[512];

    snprintf(buf, sizeof(buf), "NAME=%s\n", name);
    write(fd, buf, strlen(buf));

    snprintf(buf, sizeof(buf), "ROOTFS=%s\n", rootfs);
    write(fd, buf, strlen(buf));

    close(fd);

    return 0;
}

int sc_create_image(const char* image, const char* rootfs)
{
    if (image == NULL || rootfs == NULL)
    {
        return 1;
    }

    storage_init();

    char rootfs_path[MAX_PATH_LEN];

    snprintf(rootfs_path, MAX_PATH_LEN, "%s/%s", storage_rootfs_path(), rootfs);

    char img_path[MAX_PATH_LEN];
    char img_rootfs_path[MAX_PATH_LEN];
    char img_config_path[MAX_PATH_LEN];

    snprintf(img_path, MAX_PATH_LEN, "%s/%s", storage_image_path(), image);
    snprintf(img_rootfs_path, MAX_PATH_LEN, "%s/%s", img_path, "rootfs");
    snprintf(img_config_path, MAX_PATH_LEN, "%s/%s", img_path, "config.cfg");


    printf("Home path: %s\n", storage_home_path());
    printf("Image path: %s\n", storage_image_path());
    printf("New container path: %s/%s\n", storage_container_path(), image);

    puts("Bootstraping system");
    storage_bootstrap();
    puts("Bootstraping ended successfuly");
    
    printf("Creating dir for image %s\n", image);
    // create_image_dir(image);
    //
    if (is_path_exist(img_path))
    {
        fprintf(stderr, "Image named %s already exist\n", image);
        return 1;
    }

    if (is_path_exist(rootfs_path))
    {
        fprintf(stderr, "Rootfs named %s does't exist\n", rootfs);
        return 1;
    }

    mkdir(img_path, 0755);

    copy_dir(rootfs_path, img_rootfs_path);

    image_write_metadata(img_config_path, image, rootfs);
    

    return 0;
}
