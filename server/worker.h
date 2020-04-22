#ifndef __WORKER_H
#define __WORKER_H

#include "util.h"

void worker_init();
void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len);

#endif
