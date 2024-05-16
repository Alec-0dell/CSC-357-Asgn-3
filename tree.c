#include "tree.h"

void tree(char const *path, int depth, int *spacing, int a, int s, int *dirs, int *files)
{
    int n;
    struct dirent **direntlst;

    n = scandir(path, &direntlst, NULL, alphasort);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    for (size_t j = 0; j < n; j++)
    {
        if ((strcmp(direntlst[j]->d_name, "..") == 0) || (strcmp(direntlst[j]->d_name, ".") == 0) || (*direntlst[j]->d_name == '.' && !a))
        {
            continue;
        }

        if (direntlst[j]->d_type != DT_DIR)
        {
            *files = *files + 1;
            for (int i = 0; i < depth; i++)
            {
                if (TestBit(spacing, i))
                {
                    printf("    ");
                }
                else
                {
                    printf("|   ");
                }
            }
            if (s)
            {
                char *newpath = (char *)malloc(sizeof(char) * (strlen(path) + strlen(direntlst[j]->d_name) + 2));
                sprintf(newpath, "%s/%s", path, direntlst[j]->d_name);
                struct stat *file_status = (struct stat *)malloc(sizeof(struct stat));
                stat(newpath, file_status);
                if (j == (n - 1))
                {
                    printf("`-- [% 11ld]  %s\n", file_status->st_size, direntlst[j]->d_name);
                }
                else
                {
                    printf("|-- [% 11ld]  %s\n", file_status->st_size, direntlst[j]->d_name);
                }
                free(file_status);
                free(newpath);
            }
            else
            {
                if (j == (n - 1))
                {
                    printf("`-- %s\n", direntlst[j]->d_name);
                }
                else
                {
                    printf("|-- %s\n", direntlst[j]->d_name);
                }
            }
        }
        else if (direntlst[j]->d_type == DT_DIR)
        {
            *dirs = *dirs + 1;
            for (int i = 0; i < depth; i++)
            {
                if (TestBit(spacing, i))
                {
                    printf("    ");
                }
                else
                {
                    printf("|   ");
                }
            }

            char *newpath = (char *)malloc(sizeof(char) * (strlen(path) + strlen(direntlst[j]->d_name) + 2));
            sprintf(newpath, "%s/%s", path, direntlst[j]->d_name);

            if (s)
            {
                struct stat *file_status = (struct stat *)malloc(sizeof(struct stat));
                stat(newpath, file_status);
                if (j == (n - 1))
                {
                    printf("`-- [% 11ld]  %s\n", file_status->st_size, direntlst[j]->d_name);
                    SetBit(spacing, depth);
                    tree(newpath, depth + 1, spacing, a, s, dirs, files);
                }
                else
                {
                    printf("|-- [% 11ld]  %s\n", file_status->st_size, direntlst[j]->d_name);
                    tree(newpath, depth + 1, spacing, a, s, dirs, files);
                }
                free(file_status);
            }
            else
            {
                if (j == (n - 1))
                {
                    printf("`-- %s\n", direntlst[j]->d_name);
                    SetBit(spacing, depth);
                    tree(newpath, depth + 1, spacing, a, s, dirs, files);
                }
                else
                {
                    printf("|-- %s\n", direntlst[j]->d_name);
                    tree(newpath, depth + 1, spacing, a, s, dirs, files);
                }
            }

            free(newpath);
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        free(direntlst[i]);
    }
    free(direntlst);
}

int main(int argc, char const *argv[])
{
    int a = 0;
    int s = 0;

    int pathidx = 0;

    int dirs = 0;
    int files = 0;

    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "Usage: %s [-as] <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2)
    {
        pathidx = 1;
    }
    else
    {
        for (size_t i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-s") == 0 && s != 1)
            {
                s = 1;
            }
            else if (strcmp(argv[i], "-a") == 0 && a != 1)
            {
                a = 1;
            }
            else if (pathidx != 0)
            {
                fprintf(stderr, "Usage: %s [-as] <file_system_directory>\n", argv[0]);
                return EXIT_FAILURE;
            }
            else
            {
                pathidx = i;
            }
        }
    }

    struct stat buf;
    int r = stat(argv[pathidx], &buf);
    if (r == -1 || !S_ISDIR(buf.st_mode))
    {
        fprintf(stderr, "Usage: %s  [-as] <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *path = realpath(argv[pathidx], NULL);

    if (s)
    {
        struct stat *file_status = (struct stat *)malloc(sizeof(struct stat));
        stat(path, file_status);
        printf("[% 11ld]  %s\n", file_status->st_size, argv[pathidx]);
        free(file_status);
    }
    else
    {
        printf("%s\n", argv[pathidx]);
    }

    int A[10]; // bit array
    int i;
    for (i = 0; i < 10; i++)
        A[i] = 0; // Clear the bit array

    tree(path, 0, A, a, s, &dirs, &files);

    printf("\n");
    printf("%d directories, %d files\n", dirs, files);
    free(path);
}
