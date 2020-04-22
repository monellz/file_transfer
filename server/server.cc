#include <iostream>
#include "util.h"

int main() {
    logger_init("master");
    logger->info("server start");

    int listenfd, connfd;
    pid_t worker_pid;
    sockaddr_in_t server_addr, client_addr;
    socklen_t client_len;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    Bind(listenfd, (sockaddr_t*)&server_addr, sizeof(server_addr));

    logger->info("socket created and binded");

    Listen(listenfd, 1024);

    while (true) {
        client_len = sizeof(client_addr);
        if ((connfd = accept(listenfd, (sockaddr_t*)&client_addr, &client_len)) < 0) {
            if (errno == EINTR) continue;
            else {
                logger->error("accept error");
            }
        }

        if ((worker_pid = Fork()) == 0) {
            char buff[1000];
            //worker
            logger_init("worker");
            logger->info("worker start, peer is {}:{}", Inet_ntop(AF_INET, &client_addr.sin_addr, buff, sizeof(buff)), ntohs(client_addr.sin_port));

            logger->info("dead");
            exit(0);
        }
        //master
        logger->info("give connection to worker (pid){}", worker_pid);
    }

    /*
    int pid = fork();
    if (pid > 0) {
        logger->info("hello I am father, child is {}", pid);
        logger->warn("warn hello I am father, child is {}", pid);
        waitpid(pid, NULL, 0);
    } else {
        worker_logger_init();
        logger->info("hello I am child");
        logger->warn("warn hello I am child");
    }
    */
    return 0;
}
