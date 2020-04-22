#ifndef __WORKER_H
#define __WORKER_H

#include "util.h"

const char data_dir[] = "data";

void worker_init();
void worker_main(int connfd, sockaddr_in_t* peer_addr, socklen_t* peer_len);
void upload_file(int connfd, const char *file_name);
void download_file(int connfd, const char *file_name);

#endif
