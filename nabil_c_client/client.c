#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

// FIX 1: Match the port used in your Server code (5001)
#define SERVER_NAME "server"
#define SERVER_PORT 5001 

int main() {
    // FIX 2: Disable buffering so logs appear instantly in Docker Desktop
    setvbuf(stdout, NULL, _IONBF, 0);

    int sock;
    struct sockaddr_in server_addr;
    struct hostent *he;
    char buffer[1024] = {0};

    printf("Client starting... searching for server: %s\n", SERVER_NAME);

    // Resolve specific server container name
    he = gethostbyname(SERVER_NAME);
    if (he == NULL) {
        herror("gethostbyname failed"); // Prints why it couldn't find the server
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);

    printf("Connecting to %s on port %d...\n", SERVER_NAME, SERVER_PORT);

    // FIX 3: Added error reporting for connect
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        printf("Connected successfully! Waiting for message...\n");
        
        int bytes_received = read(sock, buffer, 1024);
        if (bytes_received > 0) {
            // Display the message received from the server
            printf("\n--- Message from C Server ---\n%s\n", buffer);
        } else {
            printf("Connected, but received no data.\n");
        }
    } else {
        perror("Connection failed"); // This will tell you IF the port is wrong or server is down
    }

    printf("Closing connection.\n");
    close(sock);
    return 0;
}
