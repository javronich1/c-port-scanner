#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/scanner.h"
#include "../include/utils.h"

// define the number of threads
#define MAX_THREADS 4

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <IP> <start_port> <end_port>\n", argv[0]);
        return 1;
    }

    // parse command-line arguments
    const char *ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    // validate port range
    if (start_port < 1 || end_port > 65535 || start_port > end_port) {
        printf("Please use a valid port range (1-65535).\n");
        return 1;
    }

    // Log the scan header before threads start
    const char *log_file = "logs/scan_results.log";
    log_scan_header(log_file, ip, start_port, end_port);

    // Calculate the total number of ports and ports per thread
    int total_ports = end_port - start_port + 1;
    int ports_per_thread = total_ports / MAX_THREADS;

    pthread_t threads[MAX_THREADS];
    ThreadArgs thread_args[MAX_THREADS];

    // threads creation
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_args[i].ip = ip;
        thread_args[i].start_port = start_port + i * ports_per_thread;
        thread_args[i].end_port = (i == MAX_THREADS - 1) ? end_port
                                                         : thread_args[i].start_port + ports_per_thread - 1;

        if (pthread_create(&threads[i], NULL, scan_ports, &thread_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // wait for all threads to complete
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // print summary of open ports
    printf("\nSummary of Open Ports:\n");
    for (int i = 0; i < open_port_count; i++) {
        printf("Port %d: OPEN\n", open_ports[i]);
    }

    printf("Scan complete.\n");
    return 0;
}