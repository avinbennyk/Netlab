#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 8080
#define BUFFERSIZE 1024

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
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
        
        // Check if number is prime
        bool prime = isPrime(num);
        
        // Prepare response message
        if (prime)
            strcpy(buffer, "Prime");
        else
            strcpy(buffer, "Not Prime");
        
        // Send prime status back to client
        sendto(serverfd, buffer, strlen(buffer), MSG_CONFIRM, (struct sockaddr *)&caddr, len);
        
        printf("%d %s\n", num, buffer);
    }
    
    close(serverfd);
    return 0;
}
