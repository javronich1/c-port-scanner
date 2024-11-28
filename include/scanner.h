#ifndef SCANNER_H
#define SCANNER_H

#include <pthread.h>

#define MAX_OPEN_PORTS 1000 // max nunber of open ports

typedef struct {
    const char *ip;
    int start_port;
    int end_port;
} ThreadArgs;

// shared resources for open ports
extern int open_ports[MAX_OPEN_PORTS];
extern int open_port_count;
extern pthread_mutex_t mutex;

void *scan_ports(void *arg);

#endif