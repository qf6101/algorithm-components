//
// Created by qfeng on 17-5-27.
//

#include "model/model_daemon.h"
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <model/xgb_predictor.h>
#include <model/adaqs_model.h>
#include <model/raw_gbdt_model.h>
#include "model/linear_model.h"
#include "log/algocomp_log.h"

namespace algocomp {

    ModelDaemon::ModelDaemon(const string model_path, const string model_type, const int update_interval) :
            _model_path(model_path), _model_type(model_type), _update_interval(update_interval) {
        _logger = spdlog::get(ACLog::_logger_name);
        // initialize path of model success file
        boost::filesystem::path p(_model_path);
        _model_succ_file_path = p.parent_path().string() + "/_MODEL_SUCCESS";

        // initialize model and daemon thread
        if (start()) {
            _logger->info(
                    "Initialize model daemon with model path={}, model type={} and update interval={}.",
                    _model_path, _model_type, _update_interval);
        } else {
            _logger->error(
                    "Fail to start model daemon with model path={}, model type={} and update interval={}.",
                    _model_path, _model_type, _update_interval);
        }
    }

    ModelDaemon::~ModelDaemon() {
        stop();
        _logger->info("Leave model daemon.");
    }

    bool ModelDaemon::start() {
        if (auto new_model = load_model()) {
            // load model
            _model.swap(new_model);
            is_model_file_modified();
            _logger->info("Finish loading core model for model daemon.");
            // initialize feature encoder
            _logger->info("Finish initializing feature encoder for model daemon. Model weight_size: {}.",
                          _model->weight_size());
            // start daemon thread
            boost::function0<void> f = boost::bind(&ModelDaemon::update_model, this);
            _daemon_thread.reset(new boost::thread(f));
            _logger->info("Finish initializing daemon thread for model daemon.");
            _logger->info("Successfully start model daemon.");
            _started = true;
            return true;
        } else {
            _logger->error("Fail to start model daemon due to core model.");
            return false;
        }
    }

    void ModelDaemon::stop() {
        if (_daemon_thread) {
            _daemon_thread->interrupt();
            _daemon_thread->join();
            _logger->info("Finish stop daemon thread for model daemon.");
        }
        _started = false;
        _logger->info("Successfully stop model daemon.");
    }

    void ModelDaemon::update_model() {
        bool continue_loop = true;
        while (continue_loop) {
            try {
                boost::this_thread::sleep(boost::posix_time::seconds(_update_interval));
                if (is_model_file_modified()) {
                    if (auto new_model = load_model()) {
                        // replace model
                        _model.swap(new_model);
                        _logger->info("Finish resetting core model for model daemon.");
                        // update feature encoder
                        _logger->info("Finish updating feature encoder for model daemon. Model weight_size: {}.",
                                      _model->weight_size());
                        _logger->info("Successfully update model for model daemon.");
                        _logger->flush();
                    }
                }
            } catch (boost::thread_interrupted &ex) {
                continue_loop = false;
                _logger->info("Interrupt the update model thread for model daemon.");
                _logger->flush();
            } catch (exception &ex) {
                _logger->error("Fail to update model for model daemon.");
                _logger->flush();
            }
        }
    }

    bool ModelDaemon::is_model_file_modified() {
        if (!boost::filesystem::exists(_model_path) || !boost::filesystem::exists(_model_succ_file_path)) {
            _logger->info("Model file does not exist or not ready for checking file status.");
            return false;
        }

        boost::system::error_code result;
        boost::filesystem::path p(_model_path);
        auto mod_time = boost::filesystem::last_write_time(p, result);
        if (result.value() == boost::system::errc::success) {
            if (mod_time != _last_mod_time) {
                auto old_time = _last_mod_time;
                _last_mod_time = mod_time;
                _logger->info("Detected model file was modified from {} to {} in model daemon.",
                              old_time, mod_time);
                return true;
            }
            LOG_DEBUG("Detected model file unchanged. Current model file was modified at {}.", _last_mod_time);
        }

        return false;
    }

    unique_ptr<Model> ModelDaemon::load_model() {
        if (!boost::filesystem::exists(_model_path) || !boost::filesystem::exists(_model_succ_file_path)) {
            _logger->error("Model file does not exist or not ready for loading {}.", _model_path);
            return nullptr;
        }

        if (_model_type == "vw-model") {
            unique_ptr<Model> output_linear_model = LinearModel::load_vw_model(_model_path);
            _logger->info("Factory loaded vw linear model for model daemon.");
            return output_linear_model;
        } else if (_model_type == "skl-model") {
            unique_ptr<Model> output_linear_model = LinearModel::load_skl_model(_model_path);
            _logger->info("Factory loaded skl linear model for model daemon.");
            return output_linear_model;
        } else if (_model_type == "xgb-predictor") {
            unique_ptr<Model> output_xgb_predictor = XgbPredictor::load_model(_model_path);
            _logger->info("Factory loaded xgboost predictor for model daemon.");
            return output_xgb_predictor;
        } else if (_model_type == "adaqs-model") {
            unique_ptr<Model> output_adaqs = AdaqsModel::load_model(_model_path);
            _logger->info("Factory loaded adaptive quickscorer for model daemon.");
            return output_adaqs;
        } else if (_model_type == "raw-gbdt-model") {
            unique_ptr<Model> output_gbtree = RawGbdtModel::load_model(_model_path);
            _logger->info("Factory loaded raw gbdt model for model daemon.");
            return output_gbtree;
        } else {
            _logger->info("Wrong model type configured: {} for model daemon.", _model_type);
            return nullptr;
        }
    }

    float ModelDaemon::score(const Eigen::SparseVector<float> &sample) {
        if (_model) {
            return _model->score(sample);
        } else {
            _logger->error("Fail to logistic predict score due to empty core model in model daemon.");
            return 0;
        }
    }

}