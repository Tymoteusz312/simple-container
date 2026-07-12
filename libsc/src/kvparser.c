#include "kvparser.h"

#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


#define BUFFER_SIZE 65536

static int is_white_ch(int ch)
{
    return ch == ' ' || ch == '\t' || ch == '\n';
}

static const char*  eat_white_ch(const char* str, const char* end)
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

int kv_parse(kv_map* kv, const char* path)
{
    if (kv == NULL)
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

    kv->count = 0;

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
            if (kv->count >= MAX_OPTION)
            {
                fprintf(stderr, "Too much variables\n");
                return -1;
            } 

            p = eat_white_ch(p, end);
            if ((p = get_val(kv->keys[kv->count], p, end)) == NULL)
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
            if ((p = get_val(kv->vars[kv->count], p, end)) == NULL)
            {
                fprintf(stderr, "Too long value");
                return -1;
            }

            ++(kv->count);
            p = eat_white_ch(p, end);
        }
    }

    if (bytes == -1)
    {
        perror("read");
        return -1;
    }

    close(fd);
    return 0;
}


int kv_load(kv_map* kv, const char* path)
{
    if (kv_parse(kv, path) == -1)
    {
        fprintf(stderr, "Error during parsing file %s\n", path);
        return -1;
    }

    return 0;
}


int kv_save(kv_map* kv, const char* path)
{
    if (kv == NULL || path == NULL) return -1;

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    for (int i = 0; i < kv->count; ++i)
    {
        char buf[MAX_KEY_LENGTH + MAX_OPT_LENGTH + 2];
        snprintf(buf, sizeof(buf), "%s=%s\n" ,kv->keys[i], kv->vars[i]);

        if (write(fd, buf, strlen(buf)) == -1)
        {
            perror("write");
            return -1;
        }
    }

    close(fd);

    return 0;
}


int kv_add(kv_map* kv, const char* key, const char* val)
{
    if (kv == NULL || key == NULL || val == NULL) return -1;

    if (kv->count >= MAX_OPTION)
        return -1;

    int idx = kv->count++;

    strlcpy(kv->keys[idx], key, MAX_KEY_LENGTH); 
    strlcpy(kv->vars[idx], val, MAX_OPT_LENGTH); 

    return 0;
}

const char* kv_get(kv_map* kv, const char* key)
{
    if (kv == NULL || key == NULL) return NULL;

    for (int i = 0; i < kv->count; ++i)
    {
        if (strcmp(kv->keys[i], key) == 0)
            return kv->vars[i];
    }
    return NULL;
}

int kv_for_each(kv_map* kv, const char* name, kv_action action)
{
    if (kv == NULL || name == NULL || action == NULL) return -1;

    int counter = 0;
    for (int i = 0; i < kv->count; ++i)
    {
        if (strcmp(kv->keys[i], name) == 0)
        {
            action(kv->vars[i]);
            ++counter;
        }
    }

    return counter;
}

