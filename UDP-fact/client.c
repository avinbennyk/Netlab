#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

int main() {
    int clientfd = 0;
    struct sockaddr_in saddr; // server_address
    char buffer[BUFFERSIZE] = {0};
    
    if ((clientfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    
    printf("Client is running...\n");
    
    int n, len = sizeof(saddr);
    
    while (1) {
        printf("\nEnter the number to calculate factorial: ");
        fgets(buffer, BUFFERSIZE, stdin);
        
        // Send number to server
        sendto(clientfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&saddr, len);
        
        // Receive factorial result from server
        n = recvfrom(clientfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr *)&saddr, (socklen_t *)&len);
        buffer[n] = '\0';
        printf("Factorial received from server: %s\n", buffer);
    }
    
    close(clientfd);
    return 0;
}
