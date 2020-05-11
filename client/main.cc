#include "util.h"

void send_file(int sockfd, const char* file_name, unsigned int is_continue) {
    msg_t msg;
    msg.type = Upload;
    long int file_len = 0;
    if (Exist(file_name)) {
        file_len = File_size(file_name);
    } else {
        logger->error("{} not exist", file_name);
        exit(0);
    }
    snprintf(msg.data, sizeof(msg.data), "%s %ld %u", file_name, file_len, is_continue);
    msg.len = offsetof(msg_t, data) + strlen(msg.data);
    logger->info("send the first msg");
    write_msg(sockfd, msg);

    //receive
    read_msg(sockfd, msg);
    logger->info("receive msg");
    long int current_file_len = 0;
    char message[PAGE_SIZE] = "";
    switch (msg.type) {
        case UploadRespond: {
            //data: current_file_len 
            sscanf(msg.data, "%ld", &current_file_len);
            logger->info("UploadRespond, current_file_len: {}", current_file_len);
            break;
        };
        case UploadRespondWarn: {
            //data: current_file_len message
            sscanf(msg.data, "%ld %s", &current_file_len, message);
            logger->warn("UploadRespondWarn, current_file_len: {}, msg: {}", current_file_len, message);
            break;
        };
        case UploadRespondErr: {
            sscanf(msg.data, "%s", message);
            logger->error("UploadRespondErr, msg: {}", message);
            exit(0);
        };
        default: {
            logger->error("unaccept msg.type: {}", (int)msg.type);
            exit(0);
        };
    }

    //send data to server
    int fd = open(file_name, O_RDONLY, 0644);
    if (is_continue) {
        long int lseek_n;
        if ((lseek_n = lseek(fd, current_file_len, SEEK_SET)) != current_file_len) {
            logger->error("{} lseek error, lseek_n: {}", file_name, lseek_n);
            exit(0);
        }
    } else {
        current_file_len = 0;
    }

    long int nread;
    long int left = file_len - current_file_len;
    msg.type = DATA;
    while (left > 0) {
        bzero(msg.data, sizeof(msg.data));
        long int prepare_read = left < PAGE_SIZE? left: PAGE_SIZE;
        if ((nread = readn(fd, msg.data, prepare_read)) < 0) {
            logger->error("read file error");
            exit(0);
        } else if (nread == 0) break;

        left -= nread;
        msg.len = offsetof(msg_t, data) + nread;
        write_msg(sockfd, msg);
        logger->info("uploading {}...{}/{}", file_name, file_len - left, file_len);
    }
    close(fd);
    logger->info("{} uploaded", file_name);
}

void receive_file(int sockfd, const char* file_name, unsigned int is_continue) {
    msg_t msg;    
    msg.type = Download;
    long int current_file_len = 0;
    bool exist = Exist(file_name);
    if (is_continue) {
        if (exist) {
            current_file_len = File_size(file_name);
            logger->info("{} exist, current file len: {}", file_name, current_file_len);
        } else {
            logger->warn("{} not exist", file_name);
        }
    }
    snprintf(msg.data, sizeof(msg.data), "%s %u %ld", file_name, is_continue, current_file_len);
    msg.len = offsetof(msg_t, data) + strlen(msg.data);
    logger->info("prepare to send the first msg");
    write_msg(sockfd, msg);
    logger->info("the first msg sended");

    //receive reply
    read_msg(sockfd, msg);
    long int file_len = 0;
    switch (msg.type) {
        case DownloadRespond: {
            sscanf(msg.data, "%ld", &file_len);

            logger->info("DownloadRespond, file_len: {}", file_len);
            break;
        };
        case DownloadRespondWarn: {
            char message[PAGE_SIZE] = "";
            sscanf(msg.data, "%ld %s", &file_len, message);
            logger->warn("DownloadRespondWarn, file_len: {}, msg: {}", file_len, message);
            break;
        };
        case DownloadRespondErr: {
            //data: message
            char message[PAGE_SIZE] = "";
            sscanf(msg.data, "%s", message);
            logger->error("DownloadRespondErr, msg: {}", message);
            exit(0);
        };
        default: {
            logger->error("unaccept msg.type: {}", (int)msg.type);
            exit(0);
        };
    }

    int fd;
    if (is_continue && exist) {
        fd = open(file_name, O_WRONLY | O_APPEND);
    } else {
        fd = open(file_name, O_WRONLY | O_CREAT, 0644);
    }
    long int left = file_len - current_file_len;
    long int nread;
    while (left > 0) {
        //long int size = sizeof(buff) < left? sizeof(buff): left;
        read_msg(sockfd, msg);
        nread = msg.len - offsetof(msg_t, data);
        //logger->info("buff: {}, len : {}, nread = {}", buff, strlen(buff), nread);

        left -= nread;
        //write
        writen(fd, msg.data, nread);
    }
    close(fd);
    logger->info("{} downloaded", file_name);
}


int main(int argc, char **argv) {
    logger_init("client");

    int sockfd;
    sockaddr_in_t server_addr;
    if (argc != 5 || (atoi(argv[2]) != 1 && atoi(argv[2]) != 2) || (atoi(argv[4]) != 0 && atoi(argv[4]) != 1)) {
        logger->error("usage: client <Server IPaddress> <Upload/Download: 1/2> <File Name> <NotContinue/Continue: 0/1");
        exit(0);
    }
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    Inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    Connect(sockfd, (sockaddr_t*)&server_addr, sizeof(server_addr));
    logger->info("connect succ");

    msg_t msg;

    char* file_name = argv[3];
    unsigned int is_continue = atoi(argv[4]);

    if (atoi(argv[2]) == 1) {
        //Upload
        logger->info("Upload, file_name: {}, is_continue: {}", file_name, is_continue);
        send_file(sockfd, file_name, is_continue);
    } else {
        //Download
        logger->info("Download, file_name: {}, is_continue: {}", file_name, is_continue);
        receive_file(sockfd, file_name, is_continue);
    }
    return 0;
}
