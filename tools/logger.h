#ifndef __LOGGER_H
#define __LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

typedef std::shared_ptr<spdlog::logger> logger_t;

extern logger_t logger;

void logger_init(const char* name);

#endif 
