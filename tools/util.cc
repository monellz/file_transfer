#include "util.h"

void read_msg(int fd, msg_t& msg) {
    bzero(msg.data, sizeof(msg.data));
    readn(fd, &msg.len, sizeof(msg.len));
    readn(fd, &msg.type, sizeof(msg.type));
    readn(fd, &msg.data, msg.len - offsetof(msg_t, data));
}

void write_msg(int fd, const msg_t& msg) {
    writen(fd, &msg, msg.len);
}
