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


    char buff[1 + MAX_FILE_NAME_BYTE + 1];
    //snprintf(buff, 1 + MAX_FILE_NAME_BYTE, "hello world");
    buff[0] = Download;
    snprintf(buff + 1, MAX_FILE_NAME_BYTE, "file name");
    writen(sockfd, buff, sizeof(buff));

    return 0;
}
