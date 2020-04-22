#include "worker.h"

void worker_init() {
    logger_init("worker");
}

void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len) {
    char buff[1 + MAX_FILE_NAME_BYTE + 1] = "";

    logger->info("worker start to communicate with {}:{}", Inet_ntop(AF_INET, &peer_addr->sin_addr, buff, sizeof(buff)), ntohs(peer_addr->sin_port)); 
    bzero(buff, sizeof(buff));
    

    //receive request type and filename
    int buff_len = readn(connfd, buff, sizeof(buff));


    logger->info("request is {}, filename is {}", REQUEST[buff[0]], buff + 1);
    
    if (buff[0] == Upload) {
        upload_file(connfd, buff + 1);
    } else if (buff[0] == Download) {
        download_file(connfd, buff + 1);

    } else {
        logger->error("unknown request type");
    }
    
    logger->info("worker finish");
}


void upload_file(int connfd, const char *file_name) {
    
}

void download_file(int connfd, const char *file_name) {
    //search for file
    char buff[PAGE_SIZE] = "";
    char file_path[1 + MAX_FILE_NAME_BYTE + 1] = "";
    snprintf(file_path, sizeof(file_path), "%s/%s", data_dir, file_name);
    if (!Exist(file_path)) {
        logger->warn("{} not exist", file_name);

        //TODO: send something?
        return;
    } else {


    }
}
