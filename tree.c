#include "tree.h"

void tree(char const *path, int depth, int spacing, int a, int s)
{
    DIR *dp = opendir(path);
    if (dp == NULL)
    {
        printf("error");
        return;
    }
    struct dirent *direntp;

    char* pname;

    direntp = readdir(dp);

    while (direntp != NULL)
    {
        if ((strcmp(direntp->d_name, "..") == 0) || (strcmp(direntp->d_name, ".") == 0) || (*direntp->d_name == '.' && !a))
        {
            direntp = readdir(dp);
            continue;
        }
        
        if (direntp->d_type != DT_DIR)
        {
            for (int i = 0; i < spacing; i++)
            {
                printf("    ");
            }
            for (int i = 0; i < (depth - spacing); i++)
            {
                printf("|   ");
            }
            pname = direntp->d_name;
            if((direntp = readdir(dp)) != NULL){
                printf("|-- %s\n", pname);
            } else {
                printf("`-- %s\n", pname);
            }
            
        } else{
            direntp = readdir(dp);
        }

    }

    rewinddir(dp);

    direntp = readdir(dp);

    while (direntp != NULL)
    {
        if ((strcmp(direntp->d_name, "..") == 0) || (strcmp(direntp->d_name, ".") == 0) || (*direntp->d_name == '.' && !a))
        {
            direntp = readdir(dp);
            continue;
        }

        if (direntp->d_type == DT_DIR)
        {
            for (int i = 0; i < spacing; i++)
            {
                printf("    ");
            }
            for (int i = 0; i < (depth - spacing); i++)
            {
                printf("|   ");
            }

            pname = direntp->d_name;
            char *newpath = (char *)malloc(sizeof(char) * (strlen(path) + strlen(pname) + 2));
            sprintf(newpath, "%s/%s", path, pname);
            if((direntp = readdir(dp)) != NULL){
                printf("|-- %s\n", pname);
                tree(newpath, depth + 1, spacing, a, s);
            } else {
                printf("`-- %s\n", pname);
                tree(newpath, depth + 1, spacing + 1, a, s);
            }
            free(newpath);
        } else{
            direntp = readdir(dp);
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
    tree(path, 0,0, a, s);
    printf("\n");
    free(path);
    return EXIT_SUCCESS;
}
