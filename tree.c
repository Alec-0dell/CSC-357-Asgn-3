#include "tree.h"


int main(int argc, char const *argv[])
{
    if(argc < 2 || argc > 4){
        fprintf(stderr, "Usage: %s <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    

    struct stat buf;
    int s = stat(argv[1], &buf);
    if( s == 1 || !S_ISDIR(buf.st_mode)){
        fprintf(stderr, "Usage: %s <file_system_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

     return EXIT_SUCCESS;
}
