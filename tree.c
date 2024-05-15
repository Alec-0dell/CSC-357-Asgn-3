#include "tree.h"

void tree(char const *path, int depth, int a, int s)
{
    DIR *dp = opendir(path);
    if (dp == NULL)
    {
        printf("error");
        return;
    }
    struct dirent *direntp;

    while ((direntp = readdir(dp)) != NULL)
    {
        if ((strcmp(direntp->d_name, "..") == 0) || (strcmp(direntp->d_name, ".") == 0) || (*direntp->d_name == '.' && !a))
        {
            continue;
        }

        for (int i = 0; i < depth; i++)
        {
            printf("|   ");
        }

        printf("|--- %s\n", direntp->d_name);

        if (direntp->d_type == DT_DIR)
        {
            char *newpath = (char *)malloc(sizeof(char) * (strlen(path) + strlen(direntp->d_name) + 2));
            sprintf(newpath, "%s/%s", path, direntp->d_name);
            tree(newpath, depth + 1, a, s);
            free(newpath);
        }
    }
    
    closedir(dp); // Close the directory outside the loop
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
    tree(path, 0, a, s);
    printf("\n");
    free(path);
    return EXIT_SUCCESS;
}
