//
// Created by qfeng on 2018/12/14
//

#include "log/algocomp_log.h"
#include <iostream>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace algocomp {

    std::string ACLog::_logger_name = "algocomp_core_logger";
    std::string ACLog::_logger_file_name = "algocomp_logs/core/log";

    bool ACLog::create_loggers(const std::string &logger_name, const std::string &logger_file_name) {
        try {
            _logger_name = logger_name;
            _logger_file_name = logger_file_name;

            spdlog::init_thread_pool(8192, 2); //queue with 8k items and 2 backing threads.

            std::vector<spdlog::sink_ptr> sinks;
#ifdef _DEBUG
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#endif
            sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt> //128M per file
                                    (_logger_file_name, 1048576 * 128, 10));

            auto logger = std::make_shared<spdlog::logger>(_logger_name, begin(sinks), end(sinks));
#ifdef _DEBUG
            logger->set_level(spdlog::level::debug);
#else
            logger->set_level(spdlog::level::info);
#endif
            spdlog::register_logger(logger);

            logger->info("Successfully initialized spdlog logger.");
            logger->flush();
        } catch (const spdlog::spdlog_ex &ex) {
            std::cout << "Spdlog initialization failed: " << ex.what() << std::endl;
        }
    };

    bool ACLog::release_loggers() {
        spdlog::drop_all();
    }

}
