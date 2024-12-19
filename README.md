

---

Multithreaded Web Server in C

This project is a lightweight multithreaded web server written in C, utilizing the pthreads library for threading and the Winsock2 (WS2_32) library for socket programming. It implements WebSocket communication using ws2tcpip.h for handling TCP/IP socket operations and custom logic for processing WebSocket frames and data (wsdata).

Features

Multithreading: Efficiently handles multiple client connections simultaneously using pthreads.

WebSocket Support: Implements WebSocket communication for real-time, bidirectional data exchange.

Winsock2 Integration: Leverages Windows Sockets API for robust network programming.

Scalable Design: Designed to support high concurrency and scalability.


Prerequisites

Windows Operating System

MinGW or Visual Studio (for compilation)

Basic knowledge of C programming and networking


Getting Started

Clone the Repository

git clone <repository-url>
cd <repository-name>

Build Instructions

1. Install a GCC compiler (e.g., MinGW or Visual Studio) on your system.


2. Compile the source code using the following command:

gcc -o server main.c -lpthread -lws2_32


3. Run the compiled binary:

./server



Configuration

Modify the server's configuration (e.g., port number, IP address) in the main.c file before compilation, if necessary.


Usage

1. Start the server by running the executable.


2. Open a WebSocket-compatible client (e.g., a browser or a WebSocket testing tool).


3. Connect to the server using the configured IP and port.




Dependencies

pthread: For multithreading

Winsock2 (ws2tcpip.h): For socket programming on Windows


