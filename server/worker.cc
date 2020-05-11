#include "worker.h"

void worker_init() {
    logger_init("worker");
}

void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len) {
    char buff[PAGE_SIZE] = "";

    logger->info("worker start to communicate with {}:{}", Inet_ntop(AF_INET, &peer_addr->sin_addr, buff, sizeof(buff)), ntohs(peer_addr->sin_port)); 
    bzero(buff, sizeof(buff));

    struct timeval to;
    to.tv_sec = 120; //2min
    to.tv_usec = 0;
    Setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (const void*)&to, sizeof(to));
    Setsockopt(connfd, SOL_SOCKET, SO_SNDTIMEO, (const void*)&to, sizeof(to));
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 60;
    Setsockopt(connfd, SOL_SOCKET, SO_LINGER, (const void*)&so_linger, sizeof(so_linger));
    

    msg_t msg;
    logger->info("prepare to receive the first msg");
    read_msg(connfd, msg);
    logger->info("the first msg received");
    switch (msg.type) {
        case Upload: {
            upload_file(connfd, msg);
            break;
        };
        case Download: {
            download_file(connfd, msg);
            break;
        };
        default: {
            logger->error("unaccepted type: {}", (int)msg.type);
            break;
        };
    }

    if (close(connfd) < 0) {
        if (errno == EWOULDBLOCK) {
            logger->error("timeout before data sended and acked");
        } else {
            logger->error("close error: {}", strerror(errno));
        }
    } else {
        logger->info("worker finish");
    }
}

void upload_file(int connfd, msg_t& msg) {
    //data: filename file_len is_continue 
    char file_path[PAGE_SIZE] = "";
    char file_name[PAGE_SIZE] = "";
    long int file_len = -1;
    unsigned int is_continue = 0;
    sscanf(msg.data, "%s %ld %u", file_name, &file_len, &is_continue);
    logger->info("request: Upload, filename: {}, file_len: {}, is_continue: {}", file_name, file_len, is_continue);
    snprintf(file_path, sizeof(file_path), "%s/%s", data_dir, file_name);
    
    long int current_file_len = 0;
    int fd;
    bool exist = Exist(file_path);
    if (exist) {
        logger->info("{} exist", file_path);
        current_file_len = File_size(file_path);
    } else {
        logger->info("{} not exist", file_path);
    }

    if (is_continue && exist) {
        fd = open(file_path, O_WRONLY | O_APPEND);
    } else {
        current_file_len = 0;
        //create new file
        fd = open(file_path, O_WRONLY | O_CREAT, 0644);
    }

    //send reply to client
    bzero(msg.data, sizeof(msg.data));
    if (exist && !is_continue) {
        logger->warn("{} has exist but client not request to continue, overwrite", file_name);
        msg.type = UploadRespondWarn;
        assert(current_file_len == 0);
        snprintf(msg.data, sizeof(msg.data), "%ld Overwrite", current_file_len);
        msg.len = strlen(msg.data) + offsetof(msg_t, data);
    } else {
        msg.type = UploadRespond;
        snprintf(msg.data, sizeof(msg.data), "%ld", current_file_len);
        msg.len = strlen(msg.data) + offsetof(msg_t, data);
    }
    write_msg(connfd, msg);


    //receive data
    long int left = file_len - current_file_len;
    long int nread;
    while (left > 0) {
        read_msg(connfd, msg);
        if (msg.type != DATA) {
            logger->error("unaccept msg.type: {}", (int)msg.type);
            exit(0);
        }
        nread = msg.len - offsetof(msg_t, data);
        left -= nread;
        //logger->info("msg.data read num: {}", nread);
        writen(fd, msg.data, nread);
    }
}

void download_file(int connfd, msg_t& msg) {
    //data: filename is_continue current_file_len
    char file_path[PAGE_SIZE] = "";
    char file_name[PAGE_SIZE] = "";
    unsigned int is_continue = 0;
    long int current_file_len = 0;
    sscanf(msg.data, "%s %u %ld", file_name, &is_continue, &current_file_len);
    logger->info("request: Download, filename: {}, is_continue: {}, current_file_len: {}", file_name, is_continue, current_file_len);
    snprintf(file_path, sizeof(file_path), "%s/%s", data_dir, file_name);

    bzero(msg.data, sizeof(msg.data));
    if (!Exist(file_path)) {
        logger->error("{} not exist, reject the request", file_name);
        msg.type = DownloadRespondErr;
        snprintf(msg.data, sizeof(msg.data), "FileNotExist");
        msg.len = offsetof(msg_t, data) + strlen(msg.data);
        write_msg(connfd, msg);
        return;
    }
    //get file len
    long int file_len = File_size(file_path);
    logger->info("{} file_len: {}", file_path, file_len);

    //try lseek
    int fd = open(file_path, O_RDONLY, 0644);
    if (is_continue) {
        long int lseek_n;
        if ((lseek_n = lseek(fd, current_file_len, SEEK_SET)) != current_file_len) {
            logger->error("{} lseek error, lseek_n: {}, err: {}", file_path, lseek_n, strerror(errno));
            msg.type = DownloadRespondErr;
            snprintf(msg.data, sizeof(msg.data), "LseekErr");
            msg.len = offsetof(msg_t, data) + strlen(msg.data);
            write_msg(connfd, msg);
            return;
        }
    } else {
        current_file_len = 0;
    }

    //now we can send a succ
    msg.type = DownloadRespond;
    snprintf(msg.data, sizeof(msg.data), "%ld", file_len);
    msg.len = offsetof(msg_t, data) + strlen(msg.data);
    write_msg(connfd, msg);


    //send file
    long int left = file_len - current_file_len;
    long int nread;
    while (left > 0) {
        bzero(msg.data, sizeof(msg.data));
        int prepare_read = left < PAGE_SIZE? left: PAGE_SIZE;
        if ((nread = readn(fd, msg.data, prepare_read)) < 0) {
            logger->error("read file error");
            exit(0);
        } else if (nread == 0) break;

        left -= nread;
        msg.type = DATA;
        msg.len = offsetof(msg_t, data) + nread;
        write_msg(connfd, msg);
        logger->info("downloading {}...{}/{}", file_name, file_len - left, file_len); 
    }
}

