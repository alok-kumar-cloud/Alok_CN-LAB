#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char ip_address[50];

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Get IP address from user (Allows localhost or actual remote IP)
    printf("Enter Server IP Address (e.g., 127.0.0.1): ");
    scanf("%s", ip_address);
    getchar(); // Clear the newline character left in input buffer

    // 2. Convert IP address from text to binary format
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to server successfully!\n\n");

    // 4. Communication loop
    while (1) {
        // Send message to Server
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        // Check if client sent "bye"
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Closing connection...\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from Server
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("\nServer disconnected.\n");
            break;
        }

        printf("Server: %s", buffer);

        // Terminate if server responded with "bye"
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server closed connection.\n");
            break;
        }
    }

    // Close socket
    close(sock);
    return 0;
}