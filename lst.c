#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define die(fmt, ...) do { printf(fmt, __VA_ARGS__); exit(1); } while (0)
#define dienz(x, fmt, ...) do { if (!x) { printf(fmt, __VA_ARGS__); exit(1); } } while (0);
#define max(x, y) ((x) > (y) ? (x) : (y))

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

    // Set behavioural flags
    bool show_all = false;
    size_t max_cols = 80;
    size_t tbl_sep = 2;

    // Get dir entries
    struct dirent *ent;
    // First time to find number of entries
    size_t n_ents = 0;
    while ((ent = readdir(dir))) {
        if (!show_all && ent->d_name[0] == '.')
            continue;
        n_ents++;
    }
    char **ents = malloc(n_ents * sizeof(*ents));
    for (size_t i = 0; i < n_ents; i++)
        ents[i] = calloc(256, sizeof(*ents[i]));

    // Second time to push into array
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

    // Sort entries in alphabetical order
    qsort(ents, n_ents, sizeof(*ents), strcmp_vp);

    // Formatting algo
    int *entname_lens = malloc(n_ents * sizeof(*entname_lens));
    for (size_t i = 0; i < n_ents; i++)
		entname_lens[i] = strlen(ents[i]);

	ssize_t opt_c = -1;
	size_t *opt_c_conf = malloc(n_ents * sizeof(*opt_c_conf));
	// we want to try with c columns per row
	for (size_t c = 1; c <= n_ents; c++) {
		size_t *maxs = calloc(c, sizeof(*maxs));
		for (size_t i = 0; i < c; i++)
			for (size_t j = i; j < n_ents; j += c)
				maxs[i] = max(maxs[i], entname_lens[j]);
		size_t tablen_c = 0;
		for (size_t i = 0; i < c; i++)
			tablen_c += maxs[i];
		printf("table length with c=%lu is %lu\n", c, tablen_c);
		if (tablen_c + (c - 1) * tbl_sep <= max_cols) {
			opt_c = c;
			memset(opt_c_conf, 0, c * sizeof(*opt_c_conf));
			memcpy(opt_c_conf, maxs, c * sizeof(*maxs));
		}
	}
        
    for (size_t i = 0; i < n_ents; i++)
        printf("%s\n", ents[i]);

	printf("%ld\n", opt_c);

    for (int i = 0; i < n_ents; i++)
        free(ents[i]);
    free(ents);
	free(entname_lens);
	free(opt_c_conf);
}
