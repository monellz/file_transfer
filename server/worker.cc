#include "worker.h"

void worker_init() {
    logger_init("worker");
    logger->info("worker init");
}

void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len) {
    char buff[1000];

    logger->info("worker start to communicate with {}:{}", Inet_ntop(AF_INET, &peer_addr->sin_addr, buff, sizeof(buff)), ntohs(peer_addr->sin_port)); 
    
    
    logger->info("worker done");
}
