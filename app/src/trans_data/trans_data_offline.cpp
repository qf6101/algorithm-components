//
// Created by qfeng on 2018/12/18.
//

#include "pipeline/pl_comp_pool.h"
#include <boost/filesystem.hpp>
#include "base64.h"
#include <boost/program_options.hpp>
#include "trans_data/data_reader.h"
#include "trans_data/data_writer.h"
#include <fstream>
#include <boost/thread/latch.hpp>

using namespace std;
using namespace algocomp;
using namespace boost::program_options;
using std::exception;

namespace fs=boost::filesystem;


bool
attrs_to_encs(const string logger_name, const string conf_fname, const string pl_name, const string name2enc_dname) {
    auto _logger = spdlog::get(logger_name);
    try {
        auto pool = PLCompPool(conf_fname);
        auto pl = pool.get_pipeline(pl_name);

        std::string msg;
        while (DataReader::read_from_local(msg)) {
            try {
                string attrs_pb;
                Base64::Decode(msg, &attrs_pb);
                auto encoded_examples = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
                for (auto &encoded: encoded_examples) {
                    if (!DataWriter::write_to_local(encoded)) {
                        _logger->error("Write encoded example fail (pl_name: {}, conf_fname: {})", pl_name, conf_fname);
                    }
                }

            } catch (const exception &ex) {
                _logger->error("Transform attributes to encodes (in loop) fail: {}", ex.what());
            }
        }

        pl->save_name2enc(name2enc_dname);
    } catch (const exception &ex) {
        _logger->error("Transform attributes to encodes fail: {}", ex.what());
        return false;
    }

    return true;
}

bool
parallel_attrs_to_encs(const string logger_name, const string conf_fname, const string pl_name, const string inp_dname,
                       const string outp_enc_dname, const string outp_n2e_fname, ctpl::thread_pool &tp,
                       int batch_size) {
    auto _logger = spdlog::get(logger_name);
    try {
        auto pool = PLCompPool(conf_fname);
        auto pl = pool.get_pipeline(pl_name);
        std::mutex out_mutex;

        for (auto &p_: fs::directory_iterator(inp_dname)) {
            auto p = p_.path();

            _logger->info("Start processing date: --> {}", p.stem().string());
            cout << "Start processing date: --> " << p.stem().string() << endl;

            std::ifstream in(p.string());
            std::ofstream out;
            out.open(outp_enc_dname + "/" + p.stem().string() + ".enc");

            vector<std::string> msgs_buf;
            std::string line;

            while (std::getline(in, line)) {
                msgs_buf.push_back(line);

                if (msgs_buf.size() >= batch_size) {
                    boost::latch count_down_latch(msgs_buf.size());

                    auto tp_func = [&pl, &_logger, &out_mutex, &count_down_latch, &out](int id, string &msg) {
                        try {
                            string attrs_pb;
                            Base64::Decode(msg, &attrs_pb);
                            auto encoded_examples = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
                            {
                                std::lock_guard<std::mutex> lock(out_mutex);
                                for (auto &encoded: encoded_examples) {
                                    out << encoded << endl;
                                }
                            }
                        } catch (exception &ex) {
                            _logger->error("Transform attributes to encodes (in loop) fail: {}", ex.what());
                        }
                        count_down_latch.count_down();
                    };

                    for (int i = 0; i < msgs_buf.size(); ++i) {
                        tp.push(tp_func, msgs_buf[i]);
                    }
                    count_down_latch.wait();
                    msgs_buf.clear();
                }
            }

            if (msgs_buf.size() > 0) {
                boost::latch count_down_latch(msgs_buf.size());

                auto tp_func = [&pl, &_logger, &out_mutex, &count_down_latch, &out](int id, string &msg) {
                    try {
                        string attrs_pb;
                        Base64::Decode(msg, &attrs_pb);
                        auto encoded_examples = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
                        {
                            std::lock_guard<std::mutex> lock(out_mutex);
                            for (auto &encoded: encoded_examples) {
                                out << encoded << endl;
                            }
                        }
                    } catch (exception &ex) {
                        _logger->error("Transform attributes to encodes (in loop) fail: {}", ex.what());
                    }
                    count_down_latch.count_down();
                };

                for (int i = 0; i < msgs_buf.size(); ++i) {
                    tp.push(tp_func, msgs_buf[i]);
                }
                count_down_latch.wait();
                msgs_buf.clear();
            }

            in.close();
            out.close();

            _logger->info("Finish processing date: --> {}", p.stem().string());
            cout << "Finish processing date: --> " << p.stem().string() << endl;
        }

        pl->save_name2enc(outp_n2e_fname);
    } catch (const exception &ex) {
        _logger->error("Transform attributes to encodes fail: {}", ex.what());
        return false;
    }

    return true;
}


bool attrs_to_encs(const string logger_name, const string conf_fname, const string pl_name, const string inp_dname,
                   const string outp_enc_dname, const string outp_n2e_fname) {
    auto _logger = spdlog::get(logger_name);
    try {
        auto pool = PLCompPool(conf_fname);
        auto pl = pool.get_pipeline(pl_name);

        for (auto &p_: fs::directory_iterator(inp_dname)) {
            auto p = p_.path();

            _logger->info("Start processing file: --> {}", p.stem().string());
            cout << "Start processing file: --> " << p.stem().string() << endl;

            std::ifstream in(p.string());
            std::ofstream out;
            out.open(outp_enc_dname + "/" + p.stem().string() + ".enc");

            auto begin_time = std::chrono::high_resolution_clock::now();
            long num_msg = 0;

            std::string msg;
            while (std::getline(in, msg)) {
                num_msg += 1;
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = (current_time - begin_time);
                if (int(diff.count() * 1000) % 60000 == 0) {
                    _logger->info("Processed " + std::to_string(num_msg) + " lines.");
                    cout << "Processed " << std::to_string(num_msg) << " lines." << endl;
                }

                try {
                    string attrs_pb;
                    Base64::Decode(msg, &attrs_pb);
                    auto encoded_examples = pl->head_to_middle(attrs_pb.c_str(), attrs_pb.length());
                    for (auto &encoded: encoded_examples) {
                        out << encoded << endl;
                    }
                } catch (const exception &ex) {
                    _logger->error("Transform attributes to encodes (in loop) fail: {}", ex.what());
                }
            }

            in.close();
            out.close();

            _logger->info("Finish processing file: --> {}", p.stem().string());
            cout << "Finish processing file: --> " << p.stem().string() << endl;
        }

        pl->save_name2enc(outp_n2e_fname);
    } catch (const exception &ex) {
        _logger->error("Transform attributes to encodes fail: {}", ex.what());
        return false;
    }

    return true;
}


int main(int argc, char **argv) {
    // Initialize parameters
    options_description desc{"Transform Feature Options"};
    desc.add_options()
            ("help,h", "Help screen")
            ("task,t", value<string>()->default_value("attrs_to_encs"), "task: attrs_to_encs")
            ("log_dname,l", value<string>()->default_value("algocomp_logs/app_trans_data"), "directory of log")
            ("conf_fname,c", value<string>()->default_value("conf/pipeline.xml"), "pipeline configuration file")
            ("pl_name,p", value<string>()->default_value("pl-1"), "name of pipeline")
            ("inp_dname,i", value<string>()->default_value(""), "input data directory")
            ("outp_enc_dname,o", value<string>()->default_value(""), "output encoded data directory")
            ("outp_n2e_fname,s", value<string>()->default_value(""), "output name2enc file")
            ("num_threads,n", value<int>()->default_value(3), "number of thread pool size")
            ("bs,b", value<int>()->default_value(5000), "batch size");

    variables_map vm;
    store(parse_command_line(argc, (const char *const *) argv, desc), vm);
    notify(vm);

    auto task = vm["task"].as<string>();
    auto log_dname = vm["log_dname"].as<string>();
    auto conf_fname = vm["conf_fname"].as<string>();
    auto pl_name = vm["pl_name"].as<string>();
    auto inp_dname = vm["inp_dname"].as<string>();
    auto outp_enc_dname = vm["outp_enc_dname"].as<string>();
    auto outp_n2e_fname = vm["outp_n2e_fname"].as<string>();
    auto num_threads = vm["num_threads"].as<int>();
    auto bs = vm["bs"].as<int>();

    // Initialize logger
    string logger_name = "algocomp_app_trans_data_logger";
    string log_fname = log_dname + "/log";
    auto log_dir = fs::path(log_dname);
    if (fs::exists(log_dir)) fs::remove_all(log_dir);
    fs::create_directories(log_dir);
    ACLog::create_loggers(logger_name, log_fname);

    auto _logger = spdlog::get(logger_name);

    ctpl::thread_pool tp(num_threads, 100000);

    if (task == "attrs_to_encs") {
        if (!parallel_attrs_to_encs(logger_name, conf_fname, pl_name, inp_dname, outp_enc_dname, outp_n2e_fname, tp,
                                    bs)) {
            _logger->error("Task of transforming attributes to encodes fail.");
            return 1;
        }
    }

    return 0;
}


