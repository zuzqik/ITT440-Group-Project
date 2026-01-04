#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_NAME "nazrinserver_c"
#define SERVER_PORT 5004

int main() {
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *he;
    char buffer[1024] = {0};

    // 1. Find the server
    he = gethostbyname(SERVER_NAME);
    if (he == NULL) {
        return 1;
    }

    // 2. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);

    // 3. Connect and Read
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        read(sock, buffer, 1024);
        printf("Random number: %s", buffer);
        fflush(stdout); 
    } else {
        printf("Error: Connection failed. Is the server running?\n");
        fflush(stdout);
    }

    close(sock);
    return 0;
}