#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5002); // Connect to C Server port [cite: 48]

    // Use container name 'server_c' if running in Docker network
    inet_pton(AF_INET, "server_c", &serv_addr.sin_addr); 

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    read(sock, buffer, 1024);
    printf("Message from C Server: %s\n", buffer);
    return 0;
}