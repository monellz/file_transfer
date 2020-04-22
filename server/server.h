#ifndef __SERVER_H
#define __SERVER_H
#include "util.h"
#include "worker.h"

void server_init();
void server_main();

void sig_worker(int signo);

#endif
