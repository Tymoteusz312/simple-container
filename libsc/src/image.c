#include "image.h"

#include "storage.h"
#include "utils.h"

#include <pwd.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

// Na poczatek zakladam ze foldery "home/$USER/.local/share/sc" istnieja, potem bede sprawdzac czy napewno

int sc_create_image(const char* image, const char* rootfs_path)
{
    if (image == NULL || rootfs_path == NULL)
    {
        return 1;
    }

    storage_init();


    printf("Home path: %s\n", storage_home_path());
    printf("Image path: %s\n", storage_image_path());
    printf("New container path: %s/%s\n", storage_container_path(), image);

    puts("Creating new folder for container");
    mkdirp(storage_container_path(), 0755);
    puts("Directories created successfuly");
    

    return 0;
}
