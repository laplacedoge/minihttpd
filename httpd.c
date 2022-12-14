
#define _GNU_SOURCE

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

const char HTTP_TEXT_404[] =
    "<!DOCTYPE html><html><body><h1>404 PAGE NOT FOUND</h1><p>We are sorry about this.</p></body></html>";

int get_line(int fd, void *buff, )

int serve_client(int fd)
{
    char buff[1024];
    int res;

    res = recv(fd, buff, 1024, 0);
    if (res > 0)
    {
        write(STDOUT_FILENO, buff, res);
    }

    res = snprintf(buff, sizeof(buff), "HTTP/2.0 404 NOT FOUND\r\n");
    send(fd, buff, res, 0);

    res = snprintf(buff, sizeof(buff), "Server: minihttpd\r\n");
    send(fd, buff, res, 0);

    res = snprintf(buff, sizeof(buff), "Content-Type: text/html; charset=utf-8\r\n");
    send(fd, buff, res, 0);

    res = snprintf(buff, sizeof(buff), "Content-Length: %lu\r\n\r\n", sizeof(HTTP_TEXT_404) - 1);
    send(fd, buff, res, 0);

    send(fd, HTTP_TEXT_404, sizeof(HTTP_TEXT_404) - 1, 0);

    printf("SENT RESPONSE!\n");
}

int main(int argc, char **argv)
{
    int server_sock_fd;
    int conn_sock_fd;
    int ret;
    socklen_t conn_sock_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock_fd == -1)
    {
        fprintf(stderr, "failed to create socket: %s\n", strerror(errno));
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(80);

    ret = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret != 0)
    {
        fprintf(stderr, "failed to bind socket: %s\n", strerror(errno));
    }

    ret = listen(server_sock_fd, 1);
    if (ret != 0)
    {
        fprintf(stderr, "failed to listen socket: %s\n", strerror(errno));
    }

    conn_sock_len = sizeof(struct sockaddr_in);
    conn_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_addr, &conn_sock_len);
    if (ret < 0)
    {
        fprintf(stderr, "failed to accept socket: %s\n", strerror(errno));
    }

    printf("GOT CONN!\n");

    serve_client(conn_sock_fd);

    close(server_sock_fd);

    return 0;
}
