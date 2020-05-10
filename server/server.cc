#include "server.h"

void server_init() {
    logger_init("master");
    logger->info("server init");

    Mkdir("data");
    signal(SIGCHLD, sig_worker);
}

void server_main() {
    logger->info("server start");
    int listenfd, connfd;
    int optval = 1;
    pid_t worker_pid;
    sockaddr_in_t server_addr, client_addr;
    socklen_t client_len;
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    Bind(listenfd, (sockaddr_t*)&server_addr, sizeof(server_addr));

    Listen(listenfd, 1024);
    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

    while (true) {
        client_len = sizeof(client_addr);
        if ((connfd = accept(listenfd, (sockaddr_t*)&client_addr, &client_len)) < 0) {
            if (errno == EINTR || errno == ECONNABORTED) continue;
            else {
                logger->error("accept error");
                exit(0);
            }
        }
        if ((worker_pid = Fork()) == 0) {
            Close(listenfd);
            worker_init();
            worker_main(connfd, &client_addr, &client_len);
            exit(0);
        }
        //master
        Close(connfd);
        logger->info("assign worker {} to a new connection", worker_pid);
    }
}

void sig_worker(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        logger->warn("worker {} terminated", pid);
    }
}
