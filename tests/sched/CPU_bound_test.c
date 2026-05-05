/* cpu_bound_test.c - Proceso que consume CPU intensivamente */
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
    printf("CPU-Bound test started (PID: %d)\n", getpid());
    printf("This process will run in an infinite loop without blocking\n");
    int c = 0;
    /* Bucle infinito que consume CPU */
    while(c < 100) {
        /* Operación que consume CPU */
        volatile int i;
        for(i = 0; i < 1000000; i++);

       print_sched_priority("cpu_bound");
       c++;
    }
    
    return 0;
}