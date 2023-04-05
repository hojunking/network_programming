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
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Connect to the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    
    char hostname[1024];
    while (1) {
        // Get the hostname from the user
        printf("Enter hostname (Q to quit): ");
        fgets(hostname, sizeof(hostname), stdin);
        hostname[strcspn(hostname, "\r\n")] = 0;
        
        // Check if the user wants to quit
        if (strcmp(hostname, "Q") == 0 || strcmp(hostname, "q") == 0) {
            break;
        }
        
        // Send the hostname to the server
        send(client_socket, hostname, strlen(hostname), 0);
        
        // Receive the IP address or error message from the server
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Server response: %s\n", buffer);
    }
    
    // Close the client socket
    close(client_socket);
    return 0;
}
