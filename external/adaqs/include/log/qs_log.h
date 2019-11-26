//
// Created by qfeng on 17-6-19.
//

#ifndef QUICKSCORER_QS_LOG_H
#define QUICKSCORER_QS_LOG_H

#include <string>
#include <spdlog/spdlog.h>

namespace quickscorer {

    class QSLog {
    public:
        static std::string _logger_name;
        static std::string _logger_file_name;

        static bool create_loggers(const std::string &logger_name, const std::string &logger_file_name);

        static bool release_loggers();
    };

#ifdef _DEBUG
#define LOG_DEBUG(...) _logger->debug(__VA_ARGS__);
#else
#define LOG_DEBUG(...) NULL
#endif

}

#endif //QUICKSCORER_QS_LOG_H
