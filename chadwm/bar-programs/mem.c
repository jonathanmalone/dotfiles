#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get the total RAM used
long long getTotalUsedRAM() {
    FILE *fp;
    char buffer[1024];
    long long total_ram = 0;
    long long free_ram = 0;
    long long buffers = 0;
    long long cached = 0;

    // Open /proc/meminfo to read memory information
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return -1; // Indicate an error
    }

    // Read the file line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (sscanf(buffer, "MemTotal: %lld kB", &total_ram) == 1) {
            // Found total RAM
        } else if (sscanf(buffer, "MemFree: %lld kB", &free_ram) == 1) {
            // Found free RAM
        } else if (sscanf(buffer, "Buffers: %lld kB", &buffers) == 1) {
            // Found buffers
        } else if (sscanf(buffer, "Cached: %lld kB", &cached) == 1) {
            // Found cached
        }

        // Break if all necessary information is found
        if (total_ram != 0 && free_ram != 0 && buffers != 0 && cached != 0) {
            break;
        }
    }

    fclose(fp);

    // Calculate used RAM: Total - Free - Buffers - Cached
    if (total_ram != 0) {
        return total_ram - free_ram - buffers - cached;
    }

    return -1; // Should not happen if /proc/meminfo is correctly parsed
}

int main() {
    long long used_ram_kb = getTotalUsedRAM();

    if (used_ram_kb != -1) {
        double used_ram_mib = (double)used_ram_kb / 1024.0; // Convert KB to MiB
        double used_ram_gib = used_ram_mib / 1024.0;       // Convert MiB to GiB

        // Check if usage is over 1 GiB
        if (used_ram_gib >= 1.0) {
            printf("%.2f GiB", used_ram_gib);
        } else {
            printf("%.2f MiB", used_ram_mib);
        }
    } else {
        printf("ERR");
        return 1; // Indicate an error
    }

    return 0; // Success
}
