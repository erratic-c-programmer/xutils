#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define die(fmt, ...) do { printf(fmt, __VA_ARGS__); exit(1); } while (0)
#define dienz(x, fmt, ...) do { if (!x) { printf(fmt, __VA_ARGS__); exit(1); } } while (0);

int strcmp_vp(const void *s1, const void *s2)
{
    return strcmp(*(const char **)s1, *(const char **)s2);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        die("Usage: %s <dirname>\n", argv[0]);

    DIR *dir = opendir(argv[1]);
    dienz(dir, "ERROR opening dir %s.\n", argv[1]);

    // set behavioural flags
    bool show_all = false;
    size_t max_cols = 80;
    size_t tbl_sep = 2;

    // get dir entries
    struct dirent *ent;
    // first time to find number of entries
    size_t n_ents = 0;
    while ((ent = readdir(dir))) {
        if (!show_all && ent->d_name[0] == '.')
            continue;
        n_ents++;
    }
    char **ents = malloc(n_ents * sizeof(*ents));
    for (size_t i = 0; i < n_ents; i++)
        ents[i] = calloc(256, sizeof(*ents[i]));

    // second time to push into array
    rewinddir(dir);
    ent = readdir(dir);
    {
        size_t i = 0;
        while ((ent = readdir(dir))) {
            if (!show_all && ent->d_name[0] == '.')
                continue;
            strcpy(ents[i++], ent->d_name);
        }
    }
    dienz(!closedir(dir), "ERROR closing dir %s.\n", argv[1]);

    // sort entries in alphabetical order
    qsort(ents, n_ents, sizeof(*ents), strcmp_vp);

    // allocate try table
    size_t tbl_slen = n_ents + max_cols + tbl_sep;
    char **tbl = malloc(n_ents * n_ents * sizeof(*tbl));
    for (size_t i = 0; i < n_ents; i++) {
        calloc
    }

    for (size_t i = 0; i < n_ents; i++)
        printf("%s\n", ents[i]);

    for (int i = 0; i < n_ents; i++)
        free(ents[i]);
    free(ents);
}
