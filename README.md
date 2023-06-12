# Linux monitoring

 Esse código utiliza a função "sysinfo" para coletar as informações de carga média do sistema nos últimos 1, 5 e 15 minutos e apresenta essas informações em tempo real na saída padrão. O loop infinito com um "sleep" de 1 segundo garante que as informações serão atualizadas a cada segundo. Além disso, trazendo informações de uso de memória, processos totais do sistema e tempo ligado.


# cpu.c

Neste código temos um scan do arquivo "/proc/cpuinfo" e passamos para a variável "frequency" utilizando a função "sscanf()" passando como parâmentro o buffer de leitura do arquivo o valor em formato de float vindo após a "string" "cpu MHz :", caso encontrado o arquivo é fechado e o valor é printado na tela com três casas após a virgula em formato "float" e assim retornando 0, finalizando o programa.

Obs.: A primeira linha do arquivo cpuinfo temos o id do núcleo do seu processador, temos as seguintes informações para cada núcleo:

``` shell
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 142
model name      : Intel(R) Core(TM) i7-10510U CPU @ 1.80GHz
stepping        : 12
microcode       : 0xf4
cpu MHz         : 4301.125
cache size      : 8192 KB
physical id     : 0
siblings        : 8
core id         : 0
cpu cores       : 4
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 22
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_>
vmx flags       : vnmi preemption_timer invvpid ept_x_only ept_ad ept_1gb flexpriority tsc_offset vtpr mtf vapic ept vpid unrestricted_guest ple pml ept_mode_based_exec
bugs            : spectre_v1 spectre_v2 spec_store_bypass swapgs itlb_multihit srbds mmio_stale_data retbleed eibrs_pbrsb
bogomips        : 4599.93
clflush size    : 64
cache_alignment : 64
address sizes   : 39 bits physical, 48 bits virtual
power management:
```

Output esperado: 
``` shell
Frequência = 4301.125 MHz
```