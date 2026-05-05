/* interactive_test.c - Proceso con comportamiento bloqueante */
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

void print_sched_priority(const char *label);

void print_sched_priority(const char *label) {
    char cmd[64];
    char buf[256];
    FILE *f;

    snprintf(cmd, sizeof(cmd), "ps -l | grep ' %d '", getpid());
    f = popen(cmd, "r");
    if (f) {
        while (fgets(buf, sizeof(buf), f))
            printf("[%s] %s", label, buf);
        pclose(f);
    }
    fflush(stdout);
}

int main() {
    int count = 0;

    printf("This process will sleep periodically\n");
    
    while(count < 100) {  /* Ejecutar por un tiempo limitado */
        print_sched_priority("sleep");
        /* Ceder la CPU voluntariamente */
        sleep(1);  /* Sleep 1 segundo - esto NO agota el quantum */
        
        count++;
    }
    
    printf("Sleep test finished\n");
    return 0;
}