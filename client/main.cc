#include "util.h"


void receive_file(int sockfd, const char* file_name, long int file_len) {
    char buff[PAGE_SIZE] = "";
    long int left;
    long int nread;

    int fd = open(file_name, O_APPEND | O_WRONLY | O_CREAT, 0644);
    bzero(buff, sizeof(buff));

    left = file_len;
    while (left > 0) {
        //long int size = sizeof(buff) < left? sizeof(buff): left;
        if ((nread = readn(sockfd, buff, sizeof(buff))) < 0) {
            logger->error("socket read error");
            exit(0);
        } else if (nread == 0) break;
        //logger->info("buff: {}, len : {}, nread = {}", buff, strlen(buff), nread);

        left -= nread;
        //write
        writen(fd, buff, nread);
    } close(fd); 
    logger->info("{} downloaded", file_name);
}

int main(int argc, char **argv) {
    logger_init("client");

    int sockfd;
    sockaddr_in_t server_addr;
    if (argc != 4 || (atoi(argv[2]) != Upload && atoi(argv[2]) != Download)) {
        logger->error("usage: client <Server IPaddress> <Upload/Download: 1/2> <File Name>");
        exit(0);
    }

    char* file_name = argv[3];

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    Inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    Connect(sockfd, (sockaddr_t*)&server_addr, sizeof(server_addr));


    char buff[PAGE_SIZE];
    int buff_len = 0;


    buff[0] = atoi(argv[2]);
    snprintf(buff + 1, PAGE_SIZE - 1, "%s", file_name);
    writen(sockfd, buff, sizeof(buff));

    //receive reply
    char r;
    long int file_len = 0;
    buff_len = readn(sockfd, buff, sizeof(buff));
    sscanf(buff, "%c%ld", &r, &file_len);

    switch (r) {
        case Succ:
            logger->info("{} len: {}", file_name, file_len);
            //acc
            //TODO: send 1 byte?
            bzero(buff, sizeof(buff));            
            writen(sockfd, buff, PAGE_SIZE);
            logger->info("acc sended");
            receive_file(sockfd, file_name, file_len);
            break;
        case NotExist:
            logger->warn("{} not exist", file_name);
            break;
        default:
            logger->error("unknown reply type");
            break;
    }

    return 0;
}
