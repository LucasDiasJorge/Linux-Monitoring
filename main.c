#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

long castTime(long sec){
    return sec/60;
}

long castRam(long b){
    return b/1000000;
}

int main()
{
    struct sysinfo si;

    while (1) {
        
        if (sysinfo(&si) != 0) {
            printf("Erro ao coletar informações de sistema\n");
            exit(1);
        }

        printf("ligado por %li minutos\n",castTime(si.uptime));

        printf("Todos os processos: %d\n",si.procs);

        printf("Memória total: %limb\n",castRam(si.totalram));

        printf("Memória livre: %limb\n",castRam(si.freeram));

        printf("Carga média do sistema nos últimos 1, 5 e 15 minutos: %.2f, %.2f, %.2f\n",
            (float)si.loads[0] / (1 << SI_LOAD_SHIFT),
            (float)si.loads[1] / (1 << SI_LOAD_SHIFT),
            (float)si.loads[2] / (1 << SI_LOAD_SHIFT));

        sleep(1);
    }

    return 0;
}