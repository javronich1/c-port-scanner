#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/scanner.h"
#include "../include/utils.h"

int open_ports[MAX_OPEN_PORTS]; // Array to store open ports for summary
int open_port_count = 0;        // Counter for open ports
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for thread-safe access

void *scan_ports(void *arg) {
    // Cast argument to ThreadArgs
    ThreadArgs *args = (ThreadArgs *)arg;
    const char *ip = args->ip;
    int start_port = args->start_port;
    int end_port = args->end_port;

    struct sockaddr_in target;
    int sock;

    const char *log_file = "logs/scan_results.log";

    target.sin_family = AF_INET;           // IPv4 protocol
    target.sin_addr.s_addr = inet_addr(ip); // Convert IP to binary

    // Validate IP address
    if (target.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        return NULL;
    }

    printf("Scanning IP: %s, Ports: %d-%d\n", ip, start_port, end_port);

    for (int port = start_port; port <= end_port; port++) {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            continue;
        }

        target.sin_port = htons(port); // Set the port

        // Attempt to connect to the port
        if (connect(sock, (struct sockaddr *)&target, sizeof(target)) == 0) {
            time_t now = time(NULL);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

            // metadata in the real-time output
            printf("Scan ID: %d | Timestamp: %s | IP: %s | Port: %d | Status: OPEN\n", open_port_count + 1, timestamp, ip, port);

            // printf("Port %d: OPEN\n", port);
            log_result(log_file, ip, port, "OPEN"); // Log the result

            // Thread-safe access to shared data
            pthread_mutex_lock(&mutex);
            if (open_port_count < MAX_OPEN_PORTS) {
                open_ports[open_port_count++] = port;
            }
            pthread_mutex_unlock(&mutex);
        }
        close(sock); // Always close the socket
    }

    return NULL; // Thread exits
}