//
// Created by AIYOJ on 2019-03-02.
//

#ifndef ALGORITHM_COMPONENTS_CALIBRATION_DAEMON_H
#define ALGORITHM_COMPONENTS_CALIBRATION_DAEMON_H


#include "calibration/calibration.h"
#include <string>
#include <boost/thread/thread.hpp>
#include <spdlog/logger.h>


namespace algocomp {
    class CalibrationDaemon {
    public:
        std::shared_ptr<spdlog::logger> _logger;

        const std::string _type;

        const std::string _fname;

        std::vector<std::string> _paths;

        std::vector<std::string> _succ_file_paths;

        std::unique_ptr<Calibration> _calibration;

        std::unique_ptr<boost::thread> _daemon_thread;

        const int _update_interval;

        std::vector<std::time_t> _last_mod_times;

        bool _started = false;

        CalibrationDaemon(const std::string &dname, const std::string &fname, const std::string &type,
                          const int &update_interval);

        virtual ~CalibrationDaemon();

        bool start();

        void stop();

        void update();

        bool is_modified();

        std::unique_ptr<algocomp::Calibration> load();

        std::optional<std::vector<float>> calibrate(const std::vector<float> &scores);

        float calibrate(float score);
    };
}

#endif //ALGORITHM_COMPONENTS_CALIBRATION_DAEMON_H
