#include <stdio.h>    // For printf, snprintf
#include <time.h>     // For time_t, struct tm, time(), localtime(), strftime
#include <stdlib.h>   // For NULL

// Function to get and format the current time
// format_string: A strftime compatible format string (e.g., "%H:%M:%S" or "%a %b %d %H:%M")
// buffer: The buffer to store the formatted time string
// buffer_size: The size of the buffer
void getCurrentFormattedTime(const char *format_string, char *buffer, size_t buffer_size) {
    time_t rawtime;
    struct tm *info;

    // Get the current time as a time_t object
    time(&rawtime);

    // Convert to local time (struct tm)
    info = localtime(&rawtime);

    // Format the time into the buffer
    // strftime returns the number of characters placed in the array,
    // not including the null terminator. Returns 0 if buffer was too small.
    if (info != NULL) {
        if (strftime(buffer, buffer_size, format_string, info) == 0) {
            // Handle error: buffer too small or invalid format string
            snprintf(buffer, buffer_size, "Time Error");
        }
    } else {
        snprintf(buffer, buffer_size, "Time Error");
    }
}

int main() {
    char time_buffer[64]; // A buffer to hold the formatted time string
    // You can change this format string to whatever you prefer.
    // Common formats:
    // "%H:%M:%S" for HH:MM:SS (24-hour)
    // "%I:%M:%S %p" for HH:MM:SS AM/PM
    // "%a %b %d %H:%M" for Day Mon DayOfMonth HH:MM (e.g., "Thu Jun 05 17:07")
    const char *desired_format = "%I:%M %p"; // Example: "Thu Jun 05 17:07:37"

    getCurrentFormattedTime(desired_format, time_buffer, sizeof(time_buffer));

    printf("%s", time_buffer);

    return 0; // Success
}
