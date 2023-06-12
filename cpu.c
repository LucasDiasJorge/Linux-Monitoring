#include <stdio.h>

int main(){
    
    FILE *fp;
    char buffer[1024];
    float frequency = 0.0;

    fp = fopen("/proc/cpuinfo", "r");

    if(fp == NULL){
        printf("Erro ao abrir o arquivo");
        return -1;
    }

    while(fgets(buffer, sizeof(buffer), fp)){
        if(sscanf(buffer,"cpu MHz : %f", &frequency) == 1){
            break;
        }
    }

    fclose(fp);

    printf("Frequência = %.3f MHz\n", frequency);

    return 0;

}