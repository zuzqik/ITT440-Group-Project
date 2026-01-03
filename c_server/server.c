#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <time.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    const char *server = "mysql";       // service name
    const char *user = "root";
    const char *password = "rootpassword";
    const char *database = "pointsdb";

    while (1) {
        conn = mysql_init(NULL);
        if (!mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0)) {
            fprintf(stderr, "Failed to connect to MySQL: %s\n", mysql_error(conn));
            sleep(5);
            continue;
        }

        // Update points for c_user
        char query[256];
        time_t now = time(NULL);
        snprintf(query, sizeof(query),
                 "INSERT INTO points_table (user, points, datetime_stamp) "
                 "VALUES('c_user', 1, FROM_UNIXTIME(%ld)) "
                 "ON DUPLICATE KEY UPDATE points = points + 1, datetime_stamp = FROM_UNIXTIME(%ld);",
                 now, now);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Update failed: %s\n", mysql_error(conn));
        } else {
            printf("Updated points for c_user\n");
        }

        mysql_close(conn);
        sleep(30); // repeat every 30s
    }

    return 0;
}
