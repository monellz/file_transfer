#include "wrap.h"


void bzero(void* dst, size_t len) {
    memset(dst, 0, len);
}

int Socket(int family, int type, int protocol) {
    int n;
    if ((n = socket(family, type, protocol)) < 0) {
        logger->error("socket error");
        exit(0);
    }
    return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        logger->error("bind error");
        exit(0);
    }
}

void Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        logger->error("listen error");
        exit(0);
    }
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (connect(fd, sa, salen) < 0) {
        logger->error("connect error");
        exit(0);
    }
}

void Close(int fd) {
    if (close(fd) == -1) {
        logger->error("close error");
        exit(0);
    }
}

ssize_t readn(int fd, void *vptr, size_t n) {
    ssize_t nread;
    size_t nleft = n;
    char *ptr = (char *)vptr;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) nread = 0;
            else return -1;
        } else if (nread == 0) break;

        nleft -= nread;
        ptr += nread;
    }

    return n - nleft;
}

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    const char *ptr = (const char*)vptr;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) nwritten = 0;
            else return -1;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}


pid_t Fork(void) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        logger->error("fork error");
    }
    return pid;
}

void Inet_pton(int family, const char* strptr, void *addrptr) {
    int n;
    if ((n = inet_pton(family, strptr, addrptr)) <= 0) {
       logger->error("inet_pton error for {}", strptr);
       exit(0);
    }
}

const char* Inet_ntop(int family, const void *addrptr, char *strptr, size_t len) {
    const char *ptr;
    if ((ptr = inet_ntop(family, addrptr, strptr, len)) == NULL) {
        logger->error("inet_ntop error");
        exit(0);
    }
    return ptr;
}


void Mkdir(const char* path) {
    if (access(path, F_OK) == 0) {
        logger->info("{} directory exists", path);
    } else {
        if (mkdir(path, 0755) != 0) {
            logger->error("mkdir error");
            exit(0);
        }
    }
}

bool Exist(const char* file_path) {
    return access(file_path, F_OK) == 0;
}
