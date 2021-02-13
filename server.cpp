#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

class server
{
private:
    /* data */
    int server_sock;
    int client_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_port;
    socklen_t client_len;

public:
    server(const char* host, int port);
    ~server();
    void listener();
    void connector();
};

server::server(const char* host, int port) {
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(port);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("failed make socket");
        exit(EXIT_FAILURE);
    }

    int binder = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (binder < 0) {
        perror("failed bind");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
}

void server::listener() {
    int lis = listen(server_sock, SOMAXCONN);
    if (lis < 0) {
        perror("failed listen");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
}

void server::connector() {
    client_len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("failed accept");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("connect from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

server::~server() {
    close(server_sock);
    close(client_sock);
}


int main() {
    server server("127.0.0.1", 8080);

    server.listener();

    server.connector();
}