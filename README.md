# C Port Scanner

A multithreaded TCP port scanner written in C, with a Flask-based web UI for running scans from the browser.

## Overview

The scanner core is implemented in C and uses POSIX threads to scan a range of TCP ports on a target host in parallel, logging results to file. A lightweight Flask server wraps the scanner with a simple web interface, validating IP/domain input and resolving hostnames before invoking the scan.

## Features

- Multithreaded TCP connect scanning across a configurable port range
- Command-line interface: `./theportscanner <IP> <start_port> <end_port>`
- Web UI (Flask) for triggering scans and viewing results in the browser
- Input validation for both IP addresses and domain names
- Scan results logged to file

## Tech Stack

- C (POSIX threads, sockets)
- Python 3 / Flask (web UI)
- HTML/CSS (frontend templates)

## Running the Project

**CLI scanner (C):**
```bash
gcc -o theportscanner src/main.c src/scanner.c src/utils.c -Iinclude -lpthread
./theportscanner <IP> <start_port> <end_port>
```

**Web UI (Flask):**
```bash
pip install -r requirements.txt
python server.py
```

## Project Context

> This project predates its public GitHub release and is being published now as part of my curated technical portfolio.
