#ifndef __UTIL_H
#define __UTIL_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "logger.h"
#include "wrap.h"

const int SERVER_PORT = 5678;
const int MAX_FILE_NAME_BYTE = 1024 - 2;
const int PAGE_SIZE = 4096;

enum request_t {
    Upload = 0,
    Download = 1
};

const char REQUEST[][10] = {"Upload", "Download"};

#endif
