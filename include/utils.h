#ifndef UTILS_H
#define UTILS_H

int log_result(const char *log_file, const char *ip, int port, const char *status);
void log_scan_header(const char *log_file, const char *ip, int start_port, int end_port);

#endif