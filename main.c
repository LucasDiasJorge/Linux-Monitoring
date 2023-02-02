#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main(){

    while (true)
    {
        system("date > data.txt");
        system("cat data.txt >> memory.txt");
        system("free -h > data.txt");
        system("cat data.txt >> memory.txt");
        sleep(1800);
    }

    return 0;

}