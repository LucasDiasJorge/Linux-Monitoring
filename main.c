#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

void get_cpu_info(struct json_object *json_obj) {
    FILE *fp;
    char buffer[128];
    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Failed to read /proc/cpuinfo");
        return;
    }
    
    struct json_object *cpu_obj = json_object_new_object();
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "model name", 10) == 0) {
            char *model = strchr(buffer, ':') + 2;
            model[strcspn(model, "\n")] = 0;  // Remove newline character
            json_object_object_add(cpu_obj, "model_name", json_object_new_string(model));
        }
    }
    
    json_object_object_add(json_obj, "cpu_info", cpu_obj);
    fclose(fp);
}

void get_memory_info(struct json_object *json_obj) {
    FILE *fp;
    char buffer[128];
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Failed to read /proc/meminfo");
        return;
    }
    
    struct json_object *mem_obj = json_object_new_object();
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strncmp(buffer, "MemTotal", 8) == 0) {
            char *mem_total = strchr(buffer, ':') + 2;
            mem_total[strcspn(mem_total, "\n")] = 0;
            json_object_object_add(mem_obj, "total_memory", json_object_new_string(mem_total));
        } else if (strncmp(buffer, "MemAvailable", 12) == 0) {
            char *mem_available = strchr(buffer, ':') + 2;
            mem_available[strcspn(mem_available, "\n")] = 0;
            json_object_object_add(mem_obj, "available_memory", json_object_new_string(mem_available));
        }
    }
    
    json_object_object_add(json_obj, "memory_info", mem_obj);
    fclose(fp);
}

void get_hd_info(struct json_object *json_obj) {
    FILE *fp = popen("df -h --output=source,size,used,avail /", "r");
    if (fp == NULL) {
        perror("Failed to execute df command");
        return;
    }
    
    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);  // Skip header
    
    struct json_object *hd_obj = json_object_new_object();
    if (fgets(buffer, sizeof(buffer), fp)) {
        char source[64], size[64], used[64], avail[64];
        sscanf(buffer, "%s %s %s %s", source, size, used, avail);
        json_object_object_add(hd_obj, "source", json_object_new_string(source));
        json_object_object_add(hd_obj, "size", json_object_new_string(size));
        json_object_object_add(hd_obj, "used", json_object_new_string(used));
        json_object_object_add(hd_obj, "available", json_object_new_string(avail));
    }
    
    json_object_object_add(json_obj, "hd_info", hd_obj);
    pclose(fp);
}

void get_process_status(struct json_object *process_obj, int pid) {
    char path[40], line[128];
    snprintf(path, 40, "/proc/%d/status", pid);
    
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Failed to open process status file");
        return;
    }
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "State", 5) == 0) {
            char *state = strchr(line, ':') + 2;
            state[strcspn(state, "\n")] = 0;
            json_object_object_add(process_obj, "state", json_object_new_string(state));
        }
    }
    
    fclose(fp);
}

void get_ip_address(struct json_object *json_obj) {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    struct json_object *ip_obj = json_object_new_object();
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
        inet_ntop(AF_INET, addr, ip, sizeof(ip));
        json_object_object_add(ip_obj, ifa->ifa_name, json_object_new_string(ip));
    }

    json_object_object_add(json_obj, "ip_addresses", ip_obj);
    freeifaddrs(ifaddr);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <pid1> <pid2> <pid3> <pid4>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    struct json_object *json_obj = json_object_new_object();
    struct json_object *processes_array = json_object_new_array();
    
    get_cpu_info(json_obj);
    get_memory_info(json_obj);
    get_hd_info(json_obj);
    get_ip_address(json_obj);
    
    for (int i = 1; i < argc; i++) {
        int pid = atoi(argv[i]);
        struct json_object *process_obj = json_object_new_object();
        json_object_object_add(process_obj, "pid", json_object_new_int(pid));
        get_process_status(process_obj, pid);
        json_object_array_add(processes_array, process_obj);
    }
    
    json_object_object_add(json_obj, "processes", processes_array);
    
    printf("%s\n", json_object_to_json_string_ext(json_obj, JSON_C_TO_STRING_PRETTY));
    
    json_object_put(json_obj);
    return EXIT_SUCCESS;
}
