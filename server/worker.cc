#include "worker.h"

void worker_init() {
    logger_init("worker");
}

void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len) {
    char buff[PAGE_SIZE] = "";

    logger->info("worker start to communicate with {}:{}", Inet_ntop(AF_INET, &peer_addr->sin_addr, buff, sizeof(buff)), ntohs(peer_addr->sin_port)); 
    bzero(buff, sizeof(buff));
    

    //receive request type and filename
    int buff_len = readn(connfd, buff, sizeof(buff));

    if (buff[0] == Upload) {
        logger->info("request is Upload, filename is {}", buff + 1);
        upload_file(connfd, buff + 1);
    } else if (buff[0] == Download) {
        logger->info("request is Download, filename is {}", buff + 1);
        download_file(connfd, buff + 1);
    } else {
        logger->error("unknown request type");
    }
    
    logger->info("worker finish");
}


void upload_file(int connfd, const char *file_name) {
    char buff[PAGE_SIZE] = "";
    char file_path[PAGE_SIZE];
    snprintf(file_path, sizeof(file_path), "%s/%s", data_dir, file_name);
    if (Exist(file_path)) {
        logger->warn("{} has exist, overwite", file_name);
        snprintf(buff, sizeof(buff), "%c0\n\n", Overwrite);
    } else {
        snprintf(buff, sizeof(buff), "%c0\n\n", Succ);
    }
    writen(connfd, buff, sizeof(buff));
    
    long int file_len;
    long int nread;
    char r;

    bzero(buff, sizeof(buff));
    nread = readn(connfd, buff, sizeof(buff));
    sscanf(buff, "%c%ld", &r, &file_len);
    logger->info("{} len: {}", file_name, file_len);

    //send acc to client
    bzero(buff, sizeof(buff));
    writen(connfd, buff, sizeof(buff));
    logger->info("acc sended");


    //receive file
    int fd = open(file_path, O_WRONLY | O_CREAT, 0644);
    bzero(buff, sizeof(buff));

    long int left = file_len;
    while (left > 0) {
        if ((nread = readn(connfd, buff, sizeof(buff))) < 0) {
            logger->error("socket read error");
            exit(0);
        } else if (nread == 0) break;

        left -= nread;
        writen(fd, buff, nread);
    }

    close(fd);
}

void download_file(int connfd, const char *file_name) {
    //search for file
    char buff[PAGE_SIZE] = "";
    char file_path[PAGE_SIZE] = "";
    snprintf(file_path, sizeof(file_path), "%s/%s", data_dir, file_name);
    if (!Exist(file_path)) {
        logger->warn("{} not exist, reject the request", file_name);

        //TODO: send something?
        snprintf(buff, sizeof(buff), "%c0\n\n", NotExist);
        writen(connfd, buff, sizeof(buff));
        return;
    } else {
        //send a reply
        long int file_len = File_size(file_path);
        snprintf(buff, sizeof(buff), "%c%ld\n\n", Succ, file_len);
        logger->info("{} len: {}", file_name, file_len);
        writen(connfd, buff, sizeof(buff));

        //bzero(buff, sizeof(buff));
        bone(buff, sizeof(buff));
        if (readn(connfd, buff, sizeof(buff)) < 0) {
            logger->error("client acc error");
            exit(0);
        }

        //send file to client
        long int left = file_len;
        long int nread;
        int fd = open(file_path, O_RDONLY, 0644);
        if (fd < 0) {
            logger->error("open file error");
            exit(0);
        }
        //bone(buff, sizeof(buff));
        //snprintf(buff, sizeof(buff), "start\n"); 
        //writen(connfd, buff, sizeof(buff));
        while (left > 0) {
            bzero(buff, sizeof(PAGE_SIZE));
            if ((nread = readn(fd, buff, PAGE_SIZE)) < 0) {
                logger->error("read file error");
                exit(0);
            } else if (nread == 0) break;
            //logger->info("read file: {}, len is {}, nread is {}", buff, strlen(buff), nread);

            left -= nread;
            writen(connfd, buff, nread);
            logger->info("downloading {}...{}/{}", file_name, file_len - left, file_len); 
        }
        close(fd);
    }
}
