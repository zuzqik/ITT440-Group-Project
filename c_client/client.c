#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    const char *server_ip = "c-server"; // service name
    int server_port = 5003;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("Socket failed"); exit(1); }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert service name to IP
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Send a request (dummy)
    send(sock, "GET_POINTS", strlen("GET_POINTS"), 0);

    // Receive response
    int valread = read(sock, buffer, 1024);
    printf("%.*s\n", valread, buffer);

    close(sock);
    return 0;
}
