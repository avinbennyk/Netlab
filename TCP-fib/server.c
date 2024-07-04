#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFERSIZE 1024

void fibonacci_series(int n, char* result) {
    if (n <= 0) {
        sprintf(result, "Invalid input");
        return;
    }
    unsigned long long int fib[n];
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    sprintf(result, "%llu", fib[0]);
    for (int i = 1; i < n; i++) {
        sprintf(result + strlen(result), ", %llu", fib[i]);
    }
}

void main() {
    int serverfd, newsocket, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFERSIZE] = {0};

    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((newsocket = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\nConnected to the client...\n");

    while (1) {
        memset(&buffer, 0, BUFFERSIZE);
        printf("\nWaiting for message...\n");
        if (recv(newsocket, buffer, BUFFERSIZE, 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "EXIT\n") == 0) {
            printf("Client initiated exit. Exiting...\n");
            break;
        }

        printf("Message received: %s\n", buffer);
        int number = atoi(buffer); // Convert the received message to an integer
        char result[BUFFERSIZE] = {0}; // Buffer to store the result
        fibonacci_series(number, result); // Calculate the Fibonacci series
        send(newsocket, result, strlen(result), 0); // Send the response
    }

    close(newsocket);
}
