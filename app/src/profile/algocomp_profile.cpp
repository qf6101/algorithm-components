//
// Created by qfeng on 2018/12/18.
//

#include "pipeline/pl_comp_pool.h"
#include <boost/filesystem.hpp>
#include "base64.h"

#ifdef _PROFILE
#include <gperftools/profiler.h>
#endif

using namespace std;
using namespace algocomp;
namespace fs=boost::filesystem;

int main(int argc, char **argv) {
    // Initialize logger
    string logger_name = "algocomp_app_eval_time_logger";
    string logger_dir_name = "algocomp_logs/app_eval_time";
    string logger_file_name = logger_dir_name + "/log";
    auto logger_dir = fs::path(logger_dir_name);
    if (fs::exists(logger_dir)) fs::remove_all(logger_dir);
    fs::create_directories(logger_dir);
    ACLog::create_loggers(logger_name, logger_file_name);

    std::ifstream in("test/test-data/encoded_attributes.txt");
    string line;
    std::getline(in, line);
    string attrs_pb;
    Base64::Decode(line, &attrs_pb);

    auto pool = PLCompPool("test/test-data/2-pipeline.xml");
    auto pl = pool.get_pipeline("pl-1");

    int repeat = 50;

#ifdef _PROFILE
    cout << "Into profile mode..." << endl;
    ProfilerStart("algocomp.prof");
#endif

    for (int i = 0; i < repeat; ++i) {
        auto encoded = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
    }

#ifdef _PROFILE
    ProfilerStop();
    cout << "Out profile mode..." << endl;
#endif

    in.close();

    return 0;
}
