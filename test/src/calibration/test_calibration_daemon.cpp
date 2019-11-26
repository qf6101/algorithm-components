//
// Created by AIYOJ on 2019-03-04.
//


#include <gtest/gtest.h>
#include "calibration/calibration_daemon.h"
#include <boost/function.hpp>
#include "calibration/isotonic_regression.h"
#include "calibration/prior_correction.h"
#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>
#include <calibration/hybrid.h>
#include "log/algocomp_log.h"


namespace algocomp_test {
    class TestCalibrationDaemon : public ::testing::Test {
    protected:
        std::unique_ptr<algocomp::CalibrationDaemon> _daemon;
        std::shared_ptr<spdlog::logger> _logger;

        TestCalibrationDaemon() {
            _logger = spdlog::get(algocomp::ACLog::_logger_name);
        }

        ~TestCalibrationDaemon() {
        }

    public:
        void thread_content() {
            bool continue_loop = true;
            while (continue_loop) {
                _logger->debug("test_calibration_daemon: pass one loop in thread content...");
                boost::this_thread::sleep(boost::posix_time::seconds(10));
            }
        }

    };

    TEST_F(TestCalibrationDaemon, load_prior_correction) {
        std::string dname = "test/test-data";
        std::string fname = "1-calibrations/1-prior-correction.txt";
        std::string type = "prior-correction";
        int update_interval = 1;
        _daemon.reset(new algocomp::CalibrationDaemon(dname, fname, type, update_interval));

        auto calibration = _daemon->load();
        auto const prior_calibration = dynamic_cast<algocomp::PriorCorrection *>(calibration.get());
        EXPECT_EQ(prior_calibration->_neg_smp_r, 0.5);
    }

    TEST_F(TestCalibrationDaemon, load_isotonic_regression) {
        std::string dname = "test/test-data";
        std::string fname = "1-calibrations/1-isotonic-regression.txt";
        std::string type = "isotonic-regression";
        int update_interval = 1;
        _daemon.reset(new algocomp::CalibrationDaemon(dname, fname, type, update_interval));

        auto calibration = _daemon->load();
        auto const isotonic_regression = dynamic_cast<algocomp::IsotonicRegression *>(calibration.get());
        EXPECT_NEAR(isotonic_regression->get(0.8), 0.15296713764868813, 1E-4);
        EXPECT_NEAR(isotonic_regression->get(0.9), 0.29661971830985917, 1E-4);
        EXPECT_NEAR(isotonic_regression->get(0.4), 0.02298094617097653, 1E-4);
    }

    TEST_F(TestCalibrationDaemon, load_hybrid) {
        std::string dname = "test/test-data";
        std::string fname = "1-calibrations/1-prior-correction.txt;1-calibrations/1-isotonic-regression.txt";
        std::string type = "hybrid";
        int update_interval = 1;
        _daemon.reset(new algocomp::CalibrationDaemon(dname, fname, type, update_interval));

        auto calibration = _daemon->load();
        auto const hybrid = dynamic_cast<algocomp::Hybrid *>(calibration.get());

        std::vector<float> scores;
        scores.push_back(0.8);
        scores.push_back(0.9);
        scores.push_back(0.4);

        std::vector<float> correct_calibrated_scores;
        std::for_each(scores.begin(), scores.end(), [this, &correct_calibrated_scores, &hybrid](const float &v) {
            float score = v / (v + (1 - v) * 2);
            correct_calibrated_scores.push_back(hybrid->get(score));
        });

        std::vector<float> calibrated_scores = calibration->calibrate(scores);
        for (int i = 0; i < scores.size(); ++i) {
            EXPECT_EQ(calibrated_scores[i], correct_calibrated_scores[i]);
        }
    }

    TEST_F(TestCalibrationDaemon, is_modified) {
        std::string dname = "test/test-data";
        std::string fname = "1-calibrations/1-isotonic-regression.txt";
        std::string type = "isotonic-regression";
        int update_interval = 1;
        _daemon.reset(new algocomp::CalibrationDaemon(dname, fname, type, update_interval));

        bool first_touch = _daemon->is_modified();
        EXPECT_FALSE(first_touch);
        boost::filesystem::remove("test/test-data/1-calibrations/1-isotonic-regression.txt.swap");
        boost::filesystem::copy("test/test-data/1-calibrations/1-isotonic-regression.txt",
                                "test/test-data/1-calibrations/1-isotonic-regression.txt.swap");
        boost::filesystem::remove("test/test-data/1-calibrations/1-isotonic-regression.txt");
        boost::filesystem::copy("test/test-data/1-calibrations/1-isotonic-regression.txt.swap",
                                "test/test-data/1-calibrations/1-isotonic-regression.txt");
        boost::filesystem::remove("test/test-data/1-calibrations/1-isotonic-regression.txt.swap");
        auto out_model = _daemon->load();
        bool second_touch = _daemon->is_modified();
        EXPECT_TRUE(second_touch);
    }

    TEST_F(TestCalibrationDaemon, boost_thread) {
        boost::function0<void> f = boost::bind(&TestCalibrationDaemon::thread_content, this);
        boost::thread thrd(f);
        _logger->debug("test_calibration_daemon: sleep 2 seconds starts...");
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        _logger->debug("test_calibration_daemon: sleep 2 seconds ends...");
        thrd.interrupt();
        thrd.join();
        _logger->debug("test_calibration_daemon: terminate thread!");
    }

    TEST_F(TestCalibrationDaemon, calibration_success_file) {
        std::string dname = "test/test-data";
        std::string fname = "1-calibrations/1-isotonic-regression.txt";
        std::string type = "isotonic-regression";
        int update_interval = 1;
        _daemon.reset(new algocomp::CalibrationDaemon(dname, fname, type, update_interval));

        _daemon->is_modified();
        EXPECT_TRUE(_daemon->load());
        boost::filesystem::rename(_daemon->_succ_file_paths[0], _daemon->_succ_file_paths[0] + ".bak");
        EXPECT_FALSE(_daemon->is_modified());
        EXPECT_FALSE(_daemon->load());
        boost::filesystem::rename(_daemon->_succ_file_paths[0] + ".bak", _daemon->_succ_file_paths[0]);
        EXPECT_FALSE(_daemon->is_modified());
        EXPECT_TRUE(_daemon->load());
    }

}