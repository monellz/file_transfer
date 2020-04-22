#ifndef __WRAP_H
#define __WRAP_H

#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "logger.h"

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;

void bzero(void* dst, size_t len);

//socket
int Socket(int family, int type, int protocol);

void Bind(int fd, const struct sockaddr *sa, socklen_t salen);

void Listen(int fd, int backlog);

void Connect(int fd, const struct sockaddr *sa, socklen_t salen);

pid_t Fork(void);

void Inet_pton(int family, const char* strptr, void *addrptr);

const char* Inet_ntop(int family, const void *addrptr, char *strptr, size_t len);


//file
void Mkdir(const char* path);

#endif
