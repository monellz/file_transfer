#ifndef __UTIL_H
#define __UTIL_H

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stddef.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

#include "wrap.h"


const int SERVER_PORT = 5678;
const int PAGE_SIZE = 4096;

/*
enum request_t {
    Upload = 1,
    Download = 2
};
*/

/*
enum reply_t {
    Succ = 1,
    Overwrite = 2,
    NotExist = 3
};
*/

//const char REQUEST[][10] = {"Upload", "Download"};

struct msg_t {
    int len;
    char type;
    char data[PAGE_SIZE];
};

enum type_t {
    Upload = 1,
    Download,
    
    UploadRespond,
    DownloadRespond,

    UploadRespondWarn,
    DownloadRespondWarn,

    UploadRespondErr,
    DownloadRespondErr,

    DATA,
};

void read_msg(int fd, msg_t& msg);
void write_msg(int fd, const msg_t& msg);

#endif
