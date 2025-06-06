#include <stdio.h>
#include <stdlib.h> // For exit()

// Function to get the CPU load average over the last minute
double getCPULoadAverage() {
    FILE *fp;
    double load_avg_1min = -1.0; // Initialize with an error value

    // Open /proc/loadavg to read load average information
    fp = fopen("/proc/loadavg", "r");
    if (fp == NULL) {
        perror("Error opening /proc/loadavg");
        return load_avg_1min; // Indicate an error
    }

    // Read the first three numbers from the file, which are the 1, 5, and 15-minute load averages
    if (fscanf(fp, "%lf", &load_avg_1min) != 1) {
        fprintf(stderr, "Error reading 1-minute load average from /proc/loadavg\n");
        load_avg_1min = -1.0; // Ensure error value if sscanf fails
    }

    fclose(fp); // Close the file

    return load_avg_1min;
}

int main() {
    double load_average = getCPULoadAverage();

    if (load_average != -1.0) {
        printf("%.2f", load_average);
    } else {
        fprintf(stderr, "Could not retrieve CPU load average information.\n");
        return 1; // Indicate an error
    }

    return 0; // Success
}

