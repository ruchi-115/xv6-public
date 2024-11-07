#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void prime_calculator(int limit) {
    int count = 0;
    for (int i = 2; i <= limit; i++) {
        if (is_prime(i)) {
            count++;
            // Synchronize the print output
            printf(1, "Prime: %d\n", i);
        }
    }
    printf(1, "Total primes: %d\n", count);
}

int main() {
    int num_processes = 3;
    int limit = 100;

    for (int i = 0; i < num_processes; i++) {
        int pid = fork();
        if (pid == 0) {
            // Child process
            int nice_value = i * 5; // Different nice values for each child
            nice(getpid(), nice_value); // Adjust priority using nice
            printf(1, "Child %d with nice value %d\n", getpid(), nice_value);
            prime_calculator(limit);
            exit();
        }
    }

    // Parent process waits for all children
    for (int i = 0; i < num_processes; i++) {
        wait();
    }

    exit();
}
