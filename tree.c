#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


void tree(char const *path, int depth, int *spacing, int a, int s)
{
    int n, numdirs;
    struct dirent **direntlst;
    numdirs = 0;

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
            for (int i = 0; i < depth; i++)
            {
                if(TestBit(spacing, i)){
                    printf("    ");
                } else {
                    printf("|   ");
                }
                
            } 
            if (j == (n - 1) && (numdirs == 0))
            {
                printf("`-- %s\n", direntlst[j]->d_name);
            }
            else
            {
                printf("|-- %s\n", direntlst[j]->d_name);
            }
        }
        else
        {
            numdirs++;
        }
    }

    for (size_t m = 0; m < n; m++)
    {
        if ((strcmp(direntlst[m]->d_name, "..") == 0) || (strcmp(direntlst[m]->d_name, ".") == 0) || (*direntlst[m]->d_name == '.' && !a))
        {
            continue;
        }

        if (direntlst[m]->d_type == DT_DIR)
        {
            numdirs--;
            for (int i = 0; i < depth; i++)
            {
                if(TestBit(spacing, i)){
                    printf("    ");
                } else {
                    printf("|   ");
                }
                
            } 

            char *newpath = (char *)malloc(sizeof(char) * (strlen(path) + strlen(direntlst[m]->d_name) + 2));
            sprintf(newpath, "%s/%s", path, direntlst[m]->d_name);
            if (m == (n - 1) || (numdirs == 0))
            {
                printf("`-- %s\n", direntlst[m]->d_name);
                SetBit(spacing, depth);
                tree(newpath, depth + 1, spacing, a, s);
            }
            else
            {
                printf("|-- %s\n", direntlst[m]->d_name);
                tree(newpath, depth + 1, spacing, a, s);
            }
            free(newpath);
        }
    }

    free(direntlst);
}

int main(int argc, char const *argv[])
{
    int a = 0;
    int s = 0;

    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "Usage: %s [-as] <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc > 2)
    {
        if (strcmp(argv[2], "-s") == 0)
            s = 1;
        else if (strcmp(argv[2], "-a") == 0)
            a = 1;
        else
        {
            fprintf(stderr, "Usage: %s [-as] <file_system_directory>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (argc == 4)
    {
        if (strcmp(argv[3], "-a") == 0 && s)
            a = 1;
        else if (strcmp(argv[3], "-s") == 0 && a)
            s = 1;
        else
        {
            fprintf(stderr, "Usage: %s [-as] <file_system_directory>\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    struct stat buf;
    int r = stat(argv[1], &buf);
    if (r == -1 || !S_ISDIR(buf.st_mode))
    {
        fprintf(stderr, "Usage: %s  [-as] <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *path = realpath(argv[1], NULL);
    printf("%s\n", argv[1]);

    int A[10]; //bit array
    int i;
    for (i = 0; i < 10; i++)
        A[i] = 0; // Clear the bit array

    tree(path, 0, A, a, s);

    printf("\n");
    free(path);


}
