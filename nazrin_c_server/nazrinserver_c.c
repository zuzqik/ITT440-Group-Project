#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 5004
#define BUFFER_SIZE 1024

int main() {
    srand(time(NULL));
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    time_t last_update = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Server is ready and listening...\n");
    fflush(stdout);

    while (1) {
        time_t now = time(NULL);

        if (difftime(now, last_update) >= 3) {
            MYSQL *conn = mysql_init(NULL);
            if (mysql_real_connect(conn, "mysql", "root", "rootpassword", "pointsdb", 3306, NULL, 0)) {
                int rand_val = rand() % 100;
                char query[256];
                sprintf(query, "INSERT INTO points_table (user, points, datetime_stamp) "
                               "VALUES ('random_user', %d, NOW()) "
                               "ON DUPLICATE KEY UPDATE points=%d, datetime_stamp=NOW()", 
                               rand_val, rand_val);
                
                mysql_query(conn, query);
                mysql_close(conn);
                last_update = now;
            }
        }

        struct timeval tv = {0, 10000}; // Check for 0.01 seconds
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        
        if (select(server_fd + 1, &readfds, NULL, NULL, &tv) > 0) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            MYSQL *conn = mysql_init(NULL);
            if (mysql_real_connect(conn, "mysql", "root", "rootpassword", "pointsdb", 3306, NULL, 0)) {
                mysql_query(conn, "SELECT points FROM points_table WHERE user='random_user'");
                MYSQL_RES *res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);
                
               if (row && row[0]) {
                    send(new_socket, row[0], strlen(row[0]), 0);
                } else {
                    send(new_socket, "?", 1, 0); 
                }
                
                mysql_free_result(res);
                mysql_close(conn);
            }
            close(new_socket);
        }
        sleep(30); 
    }
    return 0;
}