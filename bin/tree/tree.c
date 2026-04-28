#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#define IDENT  "|  "
#define BRANCH "|--"
#define MAXPATH 4096
#define MAXPREFIX 2048
static int show_hidden = 0;
static int max_depth = -1;
static int dirs_only= 0;
static int total_dirs= 0;
static int total_files=0;
/* --- PROTOTIPOS (soluciona error de función sin prototipo previo) --- */
void print_tree(const char* path, int depth, const char* prefix);
static void usage(const char *prog) __attribute__((noreturn));  // Atributo noreturn
void print_tree(const char* path, int depth, const char* prefix) 
{
    //Crear las estructuras necesarias
    DIR *dir;  
    struct dirent *entry;
    struct stat st;
    char full_path[MAXPATH];
    char new_prefix[MAXPREFIX];
    //Si se supero lo profundidad maxima retorna
    if(max_depth >= 0 && depth > max_depth)return;
    //Abre el directorio
    dir = opendir(path);
    if(!dir){
        printf("El path %s no existe\n",path);
        return;
    }
    //Iterar sobre los elementos del directorio
    while((entry = readdir(dir))!= NULL){
        //Saltar archivos ocultos si no se levanto la flag
        if(!show_hidden && entry->d_name[0] == '.')continue;
        //Construir todo el camino hasta el directorio/archivo
        snprintf(full_path,MAXPATH, "%s/%s", path, entry->d_name);
        //Llamar lstat sobre la ruta
        if (lstat(full_path, &st) < 0) { perror(full_path); continue; }
        //Validar la muestra de archivos
        if(dirs_only && !S_ISDIR(st.st_mode))continue;
        //Imprimir el nombre con la indentación que lleva
        printf("%s%s%s", prefix, BRANCH,entry->d_name);
        //Si es un directorio imprimir / y sumar 1 al total de directorios
        if(S_ISDIR(st.st_mode)){
            printf("/");
            total_dirs++;
        }
        // Sumar 1 al total de files
        else total_files++;
        //Salto de linea
        putchar('\n');
        //Si es un directorio entra en el recursivamente
        if (S_ISDIR(st.st_mode)) {
            snprintf(new_prefix, MAXPREFIX, "%s%s", prefix,IDENT);
            print_tree(full_path, depth + 1,new_prefix);
        }
    }
    //Cierra el directorio
    closedir(dir);
}
/* --- Atributo noreturn (soluciona error de función que no regresa) --- */
static void usage(const char *prog) __attribute__((noreturn));
static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s [-a] [-d] [-L depth] [path]\n"
        "  -a      Show hidden files\n"
        "  -d      Directories only\n"
        "  -L <n>  Max depth (n >= 1)\n",
        prog);
    exit(1);
}
int main(int argc, char *argv[])
{
    //inicializar variables
    const char *root = ".";  // Cambiado a const char* (soluciona error de descartar calificadores)
    int flag = 0;
    int roots[argc];
    int numOfRoots = 0;
    //Decodificar el comando argumento a argumento
    for (int i = 1; i < argc; i++) {  // i declarado dentro del for (soluciona shadowing)
        if(strcmp(argv[i], "-a") == 0) { show_hidden = 1; } //habilitar mediante flag archivos ocultos
        else if (strcmp(argv[i], "-d") == 0) { dirs_only   = 1; } //habilitar flag solo directorios
        else if (strcmp(argv[i], "-L") == 0) {//habilitar flag de maxima profundidad
            if (i + 1 >= argc) usage(argv[0]); //Se necesita un numero para la profundidad
            max_depth = atoi(argv[++i]); //Transformar a int
            if (max_depth < 1) { fprintf(stderr, "tree: -L must be >= 1\n"); exit(1); } //Verificar que es valida la profundidad
        }
        //Salvar las posiciones de todas las rutas dentro del comando(Esto es para el multienrutado)
        else if (argv[i][0] != '-') {flag = 1; roots[numOfRoots] = i; numOfRoots+= 1;}
        //Flag desconocida
        else { fprintf(stderr, "tree: unknown option '%s'\n", argv[i]); usage(argv[0]); }
    }
    if(!flag)
        print_tree(root, 1,""); // Soluciona error de pasar "" a char* (ahora prefix es const char*)
    else{
        for(int i = 0; i < numOfRoots;i++){  // i declarado dentro del for (no shadowing ahora)
            if(i >= 1) //Si hay mas de una ruta separalas
                printf("\n============================================================\n");
            root = argv[roots[i]]; //construye la ruta (válido: char* a const char*)
            print_tree(root,1,""); // aplicale el comando 
        }
    }
    printf("Total of Directorys: %d \n Total of Files: %d \n",total_dirs,total_files);
    return 0;
}