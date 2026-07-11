#include "create.h"

#include "storage.h"
#include "utils.h"
#include "image.h"

#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>


int sc_create_image(const char* image, const char* rootfs)
{
    if (image == NULL || rootfs == NULL)
    {
        return 1;
    }

    // bootstrap();
    
    path_storage st;
    init_path_storage(&st, image, NULL, rootfs);

    if (is_path_exist(st.image.base))
    {
        fprintf(stderr, "Image named %s already exist\n", image);
        return 1;
    }

    if (!is_path_exist(st.rootfs))
    {
        fprintf(stderr, "Rootfs named %s does't exist\n", rootfs);
        return 1;
    }

    create_image(&st);
    image_write_metadata(st.image.config, image, rootfs);

    return 0;
}
