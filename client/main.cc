#include "util.h"

int main(int argc, char **argv) {
    logger_init("client");

    int sockfd;
    sockaddr_in_t server_addr;
    if (argc != 2) {
        logger->error("usage: client <Server IPaddress>");
        exit(0);
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    Inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    Connect(sockfd, (sockaddr_t*)&server_addr, sizeof(server_addr));

    return 0;
}
