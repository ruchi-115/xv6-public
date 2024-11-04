#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc == 2 || argc == 3) {
        int pid, value, old_value;

        if (argc == 2) { // change nice value of current process
            pid = getpid(); // system call to get current process PID
            value = atoi(argv[1]);
        } else { // change nice value of specific process
            pid = atoi(argv[1]);
            value = atoi(argv[2]);
        }

        old_value = nice(pid, value);
        if (old_value == -1) {
            if (value < 0 || value > 5) {
                printf(2, "Error: Nice value %d is out of bounds (must be 0-5)\n", value);
            } else {
                printf(2, "Error: Process with PID %d not found\n", pid);
            }
        } else {
            printf(1, "%d %d\n", pid, old_value);
        }
    } else {
        printf(2, "Usage: nice <pid> <value> or nice <value>\n");
    }
    exit();
}
