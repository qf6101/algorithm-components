//
// Created by AIYOJ on 2019-03-02.
//


#include "calibration/calibration_daemon.h"
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include "log/algocomp_log.h"
#include "calibration/prior_correction.h"
#include "calibration/isotonic_regression.h"
#include "calibration/hybrid.h"
#include "absl/strings/str_split.h"


namespace algocomp {
    CalibrationDaemon::CalibrationDaemon(const std::string &dname,
                                         const std::string &fname,
                                         const std::string &type,
                                         const int &update_interval) : _fname(fname),
                                                                       _type(type),
                                                                       _update_interval(update_interval) {
        _logger = spdlog::get(ACLog::_logger_name);

        _paths = absl::StrSplit(fname, ';');

        std::for_each(_paths.begin(), _paths.end(), [this, &dname, &fname](std::string &v) {
            v = dname + "/" + v;

            boost::filesystem::path p(v);

            // succ_file_paths
            std::string succ_file_path = p.parent_path().string() + "/_CALIBRATION_SUCCESS";
            _succ_file_paths.push_back(succ_file_path);

            // _last_mod_times
            boost::system::error_code result;
            auto mod_time = boost::filesystem::last_write_time(p, result);
            if (result.value() == boost::system::errc::success) {
                _last_mod_times.push_back(mod_time);
            }

        });

        if (start()) {
            _logger->info(
                    "Initialize calibration daemon with calibration path={}, calibration type={} and update interval={}.",
                    fname, _type, _update_interval);
        } else {
            _logger->error(
                    "Fail to start calibration daemon with calibration path={}, calibration type={} and update interval={}.",
                    fname, _type, _update_interval);
        }
    }

    CalibrationDaemon::~CalibrationDaemon() {
        stop();
        _logger->info("Leave calibration daemon.");
    }

    bool CalibrationDaemon::start() {
        if (auto new_calibration = load()) {
            _calibration.swap(new_calibration);
            is_modified();
            _logger->info("Finish loading core calibration for calibration daemon.");

            // start daemon thread
            boost::function0<void> f = boost::bind(&CalibrationDaemon::update, this);
            _daemon_thread.reset(new boost::thread(f));
            _logger->info("Finish initializing daemon thread for calibration daemon.");
            _logger->info("Successfully start calibration daemon.");
            _started = true;
            return true;

        } else {
            _logger->error("Fail to start calibration daemon due to core calibration.");
            return false;
        }

    }

    void CalibrationDaemon::stop() {
        if (_daemon_thread) {
            _daemon_thread->interrupt();
            _daemon_thread->join();
            _logger->info("Finish stop daemon thread for calibration daemon.");
        }
        _started = false;
        _logger->info("Successfully stop calibration daemon.");
    }

    std::unique_ptr<algocomp::Calibration> CalibrationDaemon::load() {
        if (_type == "prior-correction") {
            if (_paths.size() != 1) {
                return nullptr;
            }

            if (!boost::filesystem::exists(_paths[0]) || !boost::filesystem::exists(_succ_file_paths[0])) {
                _logger->error("Calibration file does not exist or not ready for loading {}.", _fname);
                return nullptr;
            }
            std::unique_ptr<Calibration> calibration = PriorCorrection::load(_paths[0]);
            _logger->info("Factory loaded prior correction calibration for calibration daemon.");
            return calibration;
        } else if (_type == "isotonic-regression") {
            if (_paths.size() != 1) {
                return nullptr;
            }

            if (!boost::filesystem::exists(_paths[0]) || !boost::filesystem::exists(_succ_file_paths[0])) {
                _logger->error("Calibration file does not exist or not ready for loading {}.", _fname);
                return nullptr;
            }
            std::unique_ptr<Calibration> calibration = IsotonicRegression::load(_paths[0]);
            _logger->info("Factory loaded isotonic regression calibration for calibration daemon.");
            return calibration;
        } else if (_type == "hybrid") {
            if (_paths.size() != 2) {
                return nullptr;
            }

            if (!boost::filesystem::exists(_paths[0]) || !boost::filesystem::exists(_paths[1]) ||
                !boost::filesystem::exists(_succ_file_paths[0]) || !boost::filesystem::exists(_succ_file_paths[1])) {
                _logger->error("Calibration file does not exist or not ready for loading {}.", _fname);
                return nullptr;
            }
            std::unique_ptr<Calibration> calibration = Hybrid::load(_paths[0], _paths[1]);
            _logger->info(
                    "Factory loaded hybrid(prior correction + isotonic regression) calibration for calibration daemon.");
            return calibration;

        } else {
            _logger->info("Wrong calibration type configured: {} for calibration daemon.", _type);
            return nullptr;
        }

    }

    bool CalibrationDaemon::is_modified() {
        bool is_modified = false;
        for (int i = 0; i < _paths.size(); ++i) {
            if (!boost::filesystem::exists(_paths[i]) || !boost::filesystem::exists(_succ_file_paths[i])) {
                _logger->info("Calibration file does not exist or not ready for checking file status.");
                return false;
            }

            boost::system::error_code result;
            boost::filesystem::path p(_paths[i]);
            auto mod_time = boost::filesystem::last_write_time(p, result);
            if (result.value() == boost::system::errc::success) {
                if (mod_time != _last_mod_times[i]) {
                    auto old_time = _last_mod_times[i];
                    _last_mod_times[i] = mod_time;
                    _logger->info("Detected calibration file was modified from {} to {} in calibration daemon.",
                                  old_time, mod_time);
                    is_modified = true;
                }
                LOG_DEBUG("Detected calibration file unchanged. Current calibration file was modified at {}.",
                          _last_mod_times[i]);
            }
        }
        return is_modified;
    }

    std::optional<std::vector<float>> CalibrationDaemon::calibrate(const std::vector<float> &scores) {
        if (_calibration) {
            return _calibration->calibrate(scores);
        } else {
            _logger->error(
                    "Fail to logistic predict score calibration due to empty core calibration in calibration daemon.");
            return std::nullopt;
        }
    }

    float CalibrationDaemon::calibrate(float score) {
        if (_calibration) {
            return _calibration->calibrate(score);
        } else {
            _logger->error(
                    "Fail to logistic predict score calibration due to empty core calibration in calibration daemon.");
            return 0;
        }
    }

    void CalibrationDaemon::update() {
        bool continue_loop = true;
        while (continue_loop) {
            try {
                boost::this_thread::sleep(boost::posix_time::seconds(_update_interval));
                if (is_modified()) {
                    if (auto new_calibration = load()) {
                        // replace calibration
                        _calibration.swap(new_calibration);
                        _logger->info("Finish resetting core calibration for calibration daemon.");
                        _logger->info("Successfully update calibration for calibration daemon.");
                        _logger->flush();
                    }
                }

            } catch (boost::thread_interrupted &ex) {
                continue_loop = false;
                _logger->info("Interrupt the update calibration thread for calibration daemon.");
                _logger->flush();
            } catch (std::exception &ex) {
                _logger->error("Fail to update calibration for calibration daemon.");
                _logger->flush();
            }

        }
    }

}

