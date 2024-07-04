#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

// Function to calculate factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

int main() {
    int serverfd = 0;
    struct sockaddr_in saddr, caddr; // server_address, client_address
    char buffer[BUFFERSIZE] = {0};
    
    if ((serverfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    memset(&saddr, 0, sizeof(saddr));
    memset(&caddr, 0, sizeof(caddr));
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(PORT);
    
    if (bind(serverfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    printf("Server is running...\n");
    
    int n, len = sizeof(caddr);
    
    while (1) {
        n = recvfrom(serverfd, buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr *)&caddr, &len);
        buffer[n] = '\0';
        
        // Convert received message to integer
        int num = atoi(buffer);
        
        // Calculate factorial
        unsigned long long fact = factorial(num);
        
        // Convert factorial result to string
        sprintf(buffer, "%llu", fact);
        
        // Send factorial back to client
        sendto(serverfd, buffer, strlen(buffer), MSG_CONFIRM, (struct sockaddr *)&caddr, len);
        
        printf("Factorial of %d sent: %llu\n", num, fact);
    }
    
    close(serverfd);
    return 0;
}
