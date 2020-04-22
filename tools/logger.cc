#include "logger.h"

logger_t logger;

void logger_init(const char* name) {
    spdlog::set_pattern("[%H:%M:%S.%e] [%n] [pid: %5P] %^[%l]%$ %v");
    logger = spdlog::stdout_color_mt(name);
}

