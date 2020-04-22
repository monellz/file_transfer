#ifndef __LOGGER_H
#define __LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

typedef std::shared_ptr<spdlog::logger> logger_t;

logger_t logger;

void logger_init(const char* name) {
    spdlog::set_pattern("[%H:%M:%S.%e] [%n] [pid: %5P] %^[%l]%$ %v");
    logger = spdlog::stdout_color_mt(name);
}

#endif 
