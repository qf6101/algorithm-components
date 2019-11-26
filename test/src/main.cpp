#include <iostream>
#include <gtest/gtest.h>
#include <log/algocomp_log.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace algocomp;
namespace fs=boost::filesystem;

int main(int argc, char **argv) {
    string logger_name = "algocomp_unit_test_logger";
    string logger_dir_name = "algocomp_logs/unit_test";
    string logger_file_name = logger_dir_name + "/log";
    auto logger_dir = fs::path(logger_dir_name);
    if (fs::exists(logger_dir)) fs::remove_all(logger_dir);
    fs::create_directories(logger_dir);
    ACLog::create_loggers(logger_name, logger_file_name);

    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    ACLog::release_loggers();
    return result;
}
