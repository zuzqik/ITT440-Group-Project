#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 5003
#define BUFFER_SIZE 1024

void update_db() {
    MYSQL *conn = mysql_init(NULL);
    // Requirement: Connect to existing database container
    if (mysql_real_connect(conn, "mysql_db", "root", "rootpassword", "itt440-group-project-server", 0, NULL, 0)) {
        printf("Connected to Database via Docker Network!\n");
        // Requirement: Update user, increase points, and update timestamp
        char *query = "UPDATE scores SET points = points + 10, datetime_stamp = NOW() WHERE user = 'nabil_user'";
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Update Error: %s\n", mysql_error(conn));
        } else {
            printf("C Server: Database updated successfully.\n");
        }
    }
    mysql_close(conn);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Requirement: Create Socket Server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // Requirement: Port > 1024

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("C Server listening on port %d...\n", PORT);

    while(1) {
        update_db(); // Requirement: Update record
        
        // This is a basic loop. For a real project, use a timer or thread 
        // to ensure it hits exactly every 30 seconds.
        printf("Waiting for connection from C Client...\n");
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        char *message = "Data Received from C Server";
        send(new_socket, message, strlen(message), 0);
        close(new_socket);
        
        sleep(30); // Requirement: Update every 30 seconds
    }
    return 0;
}