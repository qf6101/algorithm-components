//
// Created by qfeng on 2018/12/14
//

#ifndef ALGOCOMP_AC_LOG_H
#define ALGOCOMP_AC_LOG_H


#include <string>
#include <spdlog/spdlog.h>

namespace algocomp {

    /**
     * Common log for package of algorithm components
     *
     * Define the common log resource for all the package
     */
    class ACLog {
    public:
        // Logger name
        static std::string _logger_name;
        // Log sink file
        static std::string _logger_file_name;

        // Create loggers in initialization
        static bool create_loggers(const std::string &logger_name, const std::string &logger_file_name);

        // Release loggers
        static bool release_loggers();
    };
}

#ifdef _DEBUG
#define LOG_DEBUG(...) _logger->debug(__VA_ARGS__);
#else
#define LOG_DEBUG(...) NULL
#endif


#endif //ALGOCOMP_AC_LOG_H
