#include <stdio.h>
#include <stdlib.h>
#include <string.h>    // For memset
#include <sys/statvfs.h>

// Function to get disk usage for a given mount point
int getDiskUsage(const char *mount_point, char *buffer, size_t buffer_size) {
    struct statvfs fs_info;

    // Initialize the buffer to all zeros (null characters)
    memset(buffer, 0, buffer_size);

    if (statvfs(mount_point, &fs_info) == -1) {
        perror("Error getting filesystem info with statvfs");
        snprintf(buffer, buffer_size, "Disk Error"); // This snprintf will null-terminate
        return -1;
    }

    long long total_bytes = (long long)fs_info.f_blocks * fs_info.f_frsize;
    long long free_bytes = (long long)fs_info.f_bfree * fs_info.f_frsize;
    long long available_bytes = (long long)fs_info.f_bavail * fs_info.f_frsize;
    long long used_bytes = total_bytes - free_bytes;

    double usage_percentage;
    if (total_bytes > 0) {
        usage_percentage = (double)(total_bytes - available_bytes) * 100.0 / total_bytes;
    } else {
        usage_percentage = 0.0;
    }

    double used_gib = (double)used_bytes / (1024.0 * 1024.0 * 1024.0);

    // snprintf will correctly null-terminate the string it writes.
    // The initial memset provides a clean slate before snprintf.
    snprintf(buffer, buffer_size, "%.1f%%", usage_percentage);

    return 0;
}

int main(int argc, char *argv[]) {
    char usage_buffer[128];
    const char *mount_point;

    if (argc > 1) {
        mount_point = argv[1];
    } else {
        mount_point = "/";
    }

    if (getDiskUsage(mount_point, usage_buffer, sizeof(usage_buffer)) == 0) {
        printf("%s", usage_buffer);
    } else {
        return 1;
    }

    return 0;
}
