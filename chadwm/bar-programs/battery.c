#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For access()

#define MAX_BATTERY_NAME_LEN 32
#define SYSFS_BATTERY_PATH "/sys/class/power_supply/"

// Function to read a string from a sysfs file
static char* read_sysfs_string(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Error opening sysfs file");
        return NULL;
    }

    char *buffer = NULL;
    size_t len = 0;
    ssize_t read_bytes = getline(&buffer, &len, fp); // Read the whole line
    fclose(fp);

    if (read_bytes == -1) {
        free(buffer);
        return NULL;
    }

    // Remove trailing newline if present
    if (buffer[read_bytes - 1] == '\n') {
        buffer[read_bytes - 1] = '\0';
    }

    return buffer;
}

// Function to read an integer from a sysfs file
static int read_sysfs_int(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Error opening sysfs file");
        return -1;
    }

    int value;
    if (fscanf(fp, "%d", &value) != 1) {
        fclose(fp);
        return -1; // Failed to read integer
    }
    fclose(fp);
    return value;
}

int main() {
    char battery_path[256];
    char battery_name[MAX_BATTERY_NAME_LEN];
    int found_battery = 0;

    // Common battery names in sysfs (e.g., BAT0, BAT1, BATC)
    // We try to find the first one that exists
    const char *common_battery_names[] = {
        "BAT0", "BAT1", "BATC", "CMB0", "BATP" // Add more if your system uses different names
    };
    int num_common_names = sizeof(common_battery_names) / sizeof(common_battery_names[0]);

    for (int i = 0; i < num_common_names; ++i) {
        snprintf(battery_path, sizeof(battery_path), "%s%s/capacity", SYSFS_BATTERY_PATH, common_battery_names[i]);
        // Check if the capacity file exists and is readable
        if (access(battery_path, F_OK | R_OK) == 0) {
            strncpy(battery_name, common_battery_names[i], MAX_BATTERY_NAME_LEN - 1);
            battery_name[MAX_BATTERY_NAME_LEN - 1] = '\0'; // Ensure null-termination
            found_battery = 1;
            break;
        }
    }

    if (!found_battery) {
        fprintf(stderr, "Error: No common battery name found in %s\n", SYSFS_BATTERY_PATH);
        fprintf(stderr, "You might need to adjust 'common_battery_names' in the source code.\n");
        return 1;
    }

    // --- Get Battery Percentage ---
    int capacity = -1;
    snprintf(battery_path, sizeof(battery_path), "%s%s/capacity", SYSFS_BATTERY_PATH, battery_name);
    capacity = read_sysfs_int(battery_path);

    if (capacity == -1) {
        fprintf(stderr, "Error: Could not read battery capacity.\n");
        return 1;
    }

    // --- Get Battery Status ---
    char *status = NULL;
    snprintf(battery_path, sizeof(battery_path), "%s%s/status", SYSFS_BATTERY_PATH, battery_name);
    status = read_sysfs_string(battery_path);

    if (!status) {
        fprintf(stderr, "Error: Could not read battery status.\n");
        return 1;
    }

    // --- Print Results ---
    printf("%d%% %s\n", capacity, status);

    // --- Clean Up ---
    free(status); // Free the dynamically allocated string

    return 0;
}
