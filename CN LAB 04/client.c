#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char reg_no[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    // 1. Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 2. Connect to Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to server.\n");

    // 3. Persistent query loop
    while (1) {
        printf("\nEnter Registration Number (or type 'bye' to exit): ");
        fgets(reg_no, BUFFER_SIZE, stdin);
        
        // Remove trailing newline character from fgets
        reg_no[strcspn(reg_no, "\n")] = 0;

        // Send input to server
        send(sock, reg_no, strlen(reg_no), 0);

        // Exit condition check
        if (strcmp(reg_no, "bye") == 0) {
            printf("Connection closed.\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        printf("Server Response: %s\n", buffer);
    }

    // 4. Cleanup
    close(sock);
    return 0;
}