#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/scanner.h"
#include "../include/utils.h"

int open_ports[MAX_OPEN_PORTS];
int open_port_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to detect service based on the port number
const char* detect_service(int port) {
    switch (port) {
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 443: return "HTTPS";
        case 3306: return "MySQL";
        case 3389: return "RDP";
        default: return "Unknown Service";
    }
}

void *scan_ports(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    const char *ip = args->ip;
    int start_port = args->start_port;
    int end_port = args->end_port;

    struct sockaddr_in target;
    int sock;

    const char *log_file = "logs/scan_results.log";

    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(ip);

    if (target.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        return NULL;
    }

    printf("Scanning IP: %s, Ports: %d-%d\n", ip, start_port, end_port);

    for (int port = start_port; port <= end_port; port++) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            continue;
        }

        target.sin_port = htons(port);

        if (connect(sock, (struct sockaddr *)&target, sizeof(target)) == 0) {
            const char *service = detect_service(port); // Detect service

            time_t now = time(NULL);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

            // Include service information in output
            printf("Scan ID: %d | Timestamp: %s | IP: %s | Port: %d | Status: OPEN | Service: %s\n",
                   open_port_count + 1, timestamp, ip, port, service);

            log_result(log_file, ip, port, service); // Log the result

            pthread_mutex_lock(&mutex);
            if (open_port_count < MAX_OPEN_PORTS) {
                open_ports[open_port_count++] = port;
            }
            pthread_mutex_unlock(&mutex);
        }
        close(sock);
    }

    return NULL;
}