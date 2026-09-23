#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/utils.h"

void log_scan_header(const char *log_file, const char *ip, int start_port, int end_port) {
    FILE *file = fopen(log_file, "a");
    if (!file) {
        perror("Failed to open log file for header");
        return;
    }

    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(file, "\n=== Scan Started ===\n");
    fprintf(file, "Timestamp: %s | Target IP: %s | Port Range: %d-%d\n\n",
            timestamp, ip, start_port, end_port);

    fclose(file);
}

int log_result(const char *log_file, const char *ip, int port, const char *service) {
    FILE *file = fopen(log_file, "a");
    if (!file) {
        perror("Failed to open log file");
        return -1;
    }

    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    static int scan_id = 1;
    fprintf(file, "Scan ID: %d | Timestamp: %s | IP: %s | Port: %d | Status: OPEN | Service: %s\n",
            scan_id, timestamp, ip, port, service);

    scan_id++;

    fclose(file);
    return 0;
}