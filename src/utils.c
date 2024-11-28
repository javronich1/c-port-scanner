#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/utils.h"

// function to log the scan header

void log_scan_header(const char *log_file, const char *ip, int start_port, int end_port) {
    FILE *file = fopen(log_file, "a"); // Open file in append mode
    if (!file) {
        perror("Failed to open log file for header");
        return;
    }

    // timestamp for log
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // header to log file
    fprintf(file, "\n=== Scan Started ===\n");
    fprintf(file, "Timestamp: %s | Target IP: %s | Port Range: %d-%d\n\n",
            timestamp, ip, start_port, end_port);

    fclose(file);
}

// function to log scan results to a file (file handling)

int log_result(const char *log_file, const char *ip, int port, const char *status) {
    FILE *file = fopen(log_file, "a"); // Open file in append mode
    if (!file) {
        perror("Failed to open log file");
        return -1;
    }

    // tiemstamp for log
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // write to file
    static int scan_id = 1; // Incremental scan ID
    fprintf(file, "Scan ID: %d | Timestamp: %s | IP: %s | Port: %d | Status: %s\n",
            scan_id, timestamp, ip, port, status);

    // increment id for next log
    scan_id++;

    fclose(file);
    return 0;
}