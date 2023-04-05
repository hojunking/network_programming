#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> // Add this line to include the close function

#define PORT 8000

int main() {
    // Create a socket object
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Bind the socket to a specific IP address and port number
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    
    // Listen for incoming connections
    listen(server_socket, 1);
    printf("Server listening on port %d...\n", PORT);
    
    while (1) {
        // Accept a new client connection
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        
        // Receive the hostname from the client
        char hostname[1024];
        recv(client_socket, hostname, sizeof(hostname), 0);
        hostname[strcspn(hostname, "\r\n")] = 0;
        
        // Get the IP address list for the hostname
        struct hostent* host = gethostbyname(hostname);
        if (host == NULL) {
            // If the hostname is invalid, send an error message back to the client
            char error_message[] = "Error: Invalid hostname";
            send(client_socket, error_message, sizeof(error_message), 0);
        } else {
            // Send the first IP address in the list back to the client
            char* ip_address = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
            send(client_socket, ip_address, strlen(ip_address), 0);
        }
        
        // Close the client socket
        close(client_socket);
    }
    
    // Close the server socket
    close(server_socket);
    return 0;
}
