#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define IDENT  "|  "
#define BRANCH "|--"

#define MAXPATH 4096
#define MAXPREFIX 2048

void print_tree(const char* path, char* prefix)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char full_path[MAXPATH];
    char new_prefix[MAXPREFIX];

    dir = opendir(path);
    if(!dir){
        printf("El path %s no existe",path);
        return;
    }

    while((entry = readdir(dir))!= NULL){
        
        snprintf(full_path,MAXPATH, "%s/%s", path, entry->d_name);

        if (lstat(full_path, &st) < 0) { perror(full_path); continue; }

        printf("%s %s %s", prefix, BRANCH,entry->d_name);
        putchar('\n');

        if (S_ISDIR(st.st_mode)) {
            snprintf(new_prefix, MAXPREFIX, "%s%s ", prefix,IDENT);
            print_tree(full_path, new_prefix);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    char *root = ".";
    int i;
    int flag = 0;
    for (i = 1; i < argc; i++) {
        flag = 1;
        root = argv[i];
        print_tree(root,"");
    }
    if(!flag)
        print_tree(root, "");
    return 0;
}
