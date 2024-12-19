#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle individual client connections
void* handle_client(void* arg) {
    SOCKET client_socket = *(SOCKET*)arg;
    free(arg);  // Free the dynamically allocated memory for the socket descriptor

    char buffer[BUFFER_SIZE];

    // Read the client's request
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the request string
        printf("Client Request:\n%s\n", buffer);

        // Prepare a simple HTTP response
        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
            "Hello from Multithreaded Web Server!";

        // Send the response to the client
        send(client_socket, response, strlen(response), 0);
    }

    // Close the client socket
    closesocket(client_socket);
    printf("Connection closed.\n");
    return NULL;
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        exit(EXIT_FAILURE);
    }

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_socket, 10) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Main loop to accept and handle incoming connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed: %d\n", WSAGetLastError());
            continue;
        }

        printf("New client connected.\n");

        // Allocate memory for the client socket and pass it to the thread
        SOCKET* client_socket_ptr = (SOCKET*)malloc(sizeof(SOCKET));
        if (!client_socket_ptr) {
            fprintf(stderr, "Memory allocation failed\n");
            closesocket(client_socket);
            continue;
        }
        *client_socket_ptr = client_socket;

        // Create a new thread to handle the client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, client_socket_ptr) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            free(client_socket_ptr);
            closesocket(client_socket);
        } else {
            pthread_detach(thread_id);  // Detach the thread to free resources automatically
        }
    }

    // Close the server socket (unreachable in this example)
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
