#ifndef __UTIL_H
#define __UTIL_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "wrap.h"


const int SERVER_PORT = 5678;
const int PAGE_SIZE = 4096;

enum request_t {
    Upload = 1,
    Download = 2
};

enum reply_t {
    Succ = 1,
    Overwrite = 2,
    NotExist = 3
};

const char REQUEST[][10] = {"Upload", "Download"};

#endif
