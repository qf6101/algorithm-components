//
// Created by qfeng on 17-5-31.
//

#include <gtest/gtest.h>
#include "model/model_daemon.h"
#include <boost/function.hpp>
#include <model/linear_model.h>
#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>
#include <log/algocomp_log.h>


using std::string;
using namespace algocomp;

namespace algocomp_test {

    class TestModelDaemon : public ::testing::Test {
    protected:
        unique_ptr<ModelDaemon> _daemon;
        shared_ptr<spdlog::logger> _logger;

        string model_path = "test/test-data/1-models/1-vw-model.txt";
        string model_type = "vw-model";
        int update_interval = 1;

        TestModelDaemon() {
            _logger = spdlog::get(ACLog::_logger_name);
            _daemon.reset(new ModelDaemon(model_path, model_type, update_interval));
        }

        ~TestModelDaemon() {
        }

    public:
        void thread_content() {
            bool continue_loop = true;
            while(continue_loop) {
                _logger->debug("test_model_daemon: pass one loop in thread content...");
                boost::this_thread::sleep(boost::posix_time::seconds(10));
            }
        }
    };

    TEST_F(TestModelDaemon, load_model) {
        auto out_model = _daemon->load_model();
        auto const out_linear_model = dynamic_cast<LinearModel*>(out_model.get());
        EXPECT_EQ(out_linear_model->weight_size(), pow(2, 18));
        EXPECT_NEAR(out_linear_model->get(11), -0.00402038, 1E-8);
        EXPECT_NEAR(out_linear_model->get(169320), 0.0352785, 1E-8);
        EXPECT_NEAR(out_linear_model->get(262110), 0.0423157, 1E-8);
    }

    TEST_F(TestModelDaemon, is_model_file_modified) {
        bool first_touch = _daemon->is_model_file_modified();
        EXPECT_FALSE(first_touch);
        boost::filesystem::remove("test/test-data/vw_model.txt.swap");
        boost::filesystem::copy("test/test-data/1-models/1-vw-model.txt", "test/test-data/1-models/1-vw-model.txt.swap");
        boost::filesystem::remove("test/test-data/1-models/1-vw-model.txt");
        boost::filesystem::copy("test/test-data/1-models/1-vw-model.txt.swap", "test/test-data/1-models/1-vw-model.txt");
        boost::filesystem::remove("test/test-data/1-models/1-vw-model.txt.swap");
        auto out_model = _daemon->load_model();
        bool second_touch = _daemon->is_model_file_modified();
        EXPECT_TRUE(second_touch);
    }

    TEST_F(TestModelDaemon, boost_thread) {
        boost::function0<void> f = boost::bind(&TestModelDaemon::thread_content, this);
        boost::thread thrd(f);
        _logger->debug("test_model_daemon: sleep 2 seconds starts...");
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        _logger->debug("test_model_daemon: sleep 2 seconds ends...");
        thrd.interrupt();
        thrd.join();
        _logger->debug("test_model_daemon: terminate thread!");
    }

    TEST_F(TestModelDaemon, model_success_file) {
        _daemon->is_model_file_modified();
        EXPECT_TRUE(_daemon->load_model());
        boost::filesystem::rename(_daemon->_model_succ_file_path, _daemon->_model_succ_file_path + ".bak");
        EXPECT_FALSE(_daemon->is_model_file_modified());
        EXPECT_FALSE(_daemon->load_model());
        boost::filesystem::rename(_daemon->_model_succ_file_path + ".bak", _daemon->_model_succ_file_path);
        EXPECT_FALSE(_daemon->is_model_file_modified());
        EXPECT_TRUE(_daemon->load_model());
    }

} //namespace
