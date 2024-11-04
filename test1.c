#include "types.h"
#include "user.h"

int main(void) {
    int pid = fork();
    if (pid == 0) {
        // Child process
        printf(1, "Child PID: %d, setting nice value to 10\n", getpid());
        nice(getpid(), 10); // example test
        printf(1, "Child exiting\n");
        exit();
    } else {
        // Parent process
        wait();
        printf(1, "Parent completed\n");
    }
    exit();
}
