#include "config.h"

#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


#define BUFFER_SIZE 65536

int is_white_ch(int ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

const char*  eat_white_ch(const char* str, const char* end)
{
    while (str < end && is_white_ch(*str))
        ++str;
    return str;
}

static const char* get_val(char* result, const char* ch, const char* end)
{
    int len = 0;

    while (ch < end && !is_white_ch(*ch) && *ch != '=')
    {
        if(len + 1 >= MAX_KEY_LENGTH)
            return NULL;

        result[len++] = *(ch++);
    }

    result[len] = '\0';
    return ch;
}

int config_parse(config* cfg, const char* path)
{
    if (cfg == NULL)
    {
        fprintf(stderr, "Config cant be NULL]\n");
        return -1;
    }
    if (path == NULL)
    {
        fprintf(stderr, "Path cant be NULL]\n");
        return -1;
    }

    if (!is_path_exist(path))
    {
        fprintf(stderr, "File %s doesn't exist\n", path);
        return -1;
    }

    cfg->path = path; 
    cfg->count = 0;

    int fd = open(path, O_RDONLY);

    if (fd == -1)
    {
        perror("get config open");
        return -1;
    }

    char buf[BUFFER_SIZE];
    ssize_t bytes = 0;

    while((bytes = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        const char* end = buf + bytes;
        const char* p = buf;
        while (p < end)
        {
            if (cfg->count >= MAX_OPTION)
            {
                fprintf(stderr, "Too much variables\n");
                return -1;
            } 

            p = eat_white_ch(p, end);
            if ((p = get_val(cfg->keys[cfg->count], p, end)) == NULL)
            {
                fprintf(stderr, "Too long key");
                return -1;
            }
            p = eat_white_ch(p, end);
            
            if (p == end || *p != '=')
            {
                fprintf(stderr, "Missing demlimiter '=' charatcer:  %x, index = %ld", *p, end - p);
                return -1; 
            }
            ++p;
            
            p = eat_white_ch(p, end);
            if ((p = get_val(cfg->vars[cfg->count], p, end)) == NULL)
            {
                fprintf(stderr, "Too long value");
                return -1;
            }

            ++(cfg->count);
            p = eat_white_ch(p, end);
        }
    }

    if (bytes == -1)
    {
        perror("read");
        return -1;
    }

    close(fd);

    for (int i = 0; i < cfg->count; ++i)
    {
        printf("Option %s = %s\n", cfg->keys[i], cfg->vars[i]);
    }

    return 0;
}


const char* config_get(config* cfg, const char* key)
{
    if (cfg == NULL || key == NULL) return NULL;

    for (int i = 0; i < cfg->count; ++i)
    {
        if (strcmp(cfg->keys[i], key) == 0)
            return cfg->vars[i];
    }
    return NULL;
}

int config_for_each(config* cfg, const char* name, cfg_action  action)
{
    if (cfg == NULL || name == NULL || action == NULL) return -1;

    int counter = 0;
    for (int i = 0; i < cfg->count; ++i)
    {
        if (strcmp(cfg->keys[i], name) == 0)
        {
            action(cfg->vars[i]);
            ++counter;
        }
    }

    return counter;
}

