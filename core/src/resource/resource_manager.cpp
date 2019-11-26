//
// Created by qfeng on 2018-12-27.
//

#include "resource/resource_manager.h"
#include "attribute/attr_transer.h"
#include "feature/feat_transer.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include "feature/bucketize_op.h"
#include "feature/log_op.h"
#include "feature/cross_op.h"
#include "feature/compare_op.h"
#include <memory>
#include "encode/feature_hash.h"
#include "encode/feature_map.h"
#include "model/model_daemon.h"
#include "calibration/calibration_daemon.h"

using namespace std;
namespace pt = boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace algocomp {

    const shared_ptr<AttrTranser> ResourceManager::get_attr_transer(const string &name) {
        auto at_found = _attr_transers.find(name);
        if (at_found != _attr_transers.end()) {
            return at_found->second;
        } else {
            _logger->error("No matched attribute transformer found: {}.", name);
            return nullptr;
        }
    }

    const shared_ptr<FeatTranser> ResourceManager::get_feat_transer(const string &name) {
        auto ft_found = _feat_transers.find(name);
        if (ft_found != _feat_transers.end()) {
            return ft_found->second;
        } else {
            _logger->error("No matched feature transformer found: {}.", name);
            return nullptr;
        }
    }

    const shared_ptr<FeatureEncoder> ResourceManager::get_feat_encoder(const string &name) {
        auto enc_found = _feat_encoders.find(name);
        if (enc_found != _feat_encoders.end()) {
            return enc_found->second;
        } else {
            _logger->error("No matched feature encoder found: {}.", name);
            return nullptr;
        }
    }

    const shared_ptr<ModelDaemon> ResourceManager::get_model(const string &name) {
        auto model_found = _models.find(name);
        if (model_found != _models.end()) {
            return model_found->second;
        } else {
            _logger->error("No matched model found: {}.", name);
            return nullptr;
        }
    }

    const shared_ptr<CalibrationDaemon> ResourceManager::get_calibration(const string &name) {
        auto calibration_found = _calibrations.find(name);
        if (calibration_found != _calibrations.end()) {
            return calibration_found->second;
        } else {
            _logger->error("No matched calibration found: {}.", name);
            return nullptr;
        }
    }

    bool ResourceManager::create_base_feat_ops() {
        _feat_ops["bucketize"] = make_shared<BucketizeOp>();
        _feat_ops["log"] = make_shared<LogOp>();
        _feat_ops["cross"] = make_shared<CrossOp>();
        _feat_ops["crossall"] = make_shared<CrossOp>();
        _feat_ops["comp"] = make_shared<CompareOp>();
    }

    ResourceManager::ResourceManager(const string &conf_fname, const string &conf_dname, ThreadType thread_type) {
        // initialize logger
        _logger = spdlog::get(ACLog::_logger_name);

        try {
            pt::ptree tree;
            pt::read_xml(conf_dname + "/" + conf_fname, tree, trim_whitespace | no_comments);

            // thread pool
            {
                auto num_threads = tree.get<int>("resources.thread-pool.num_threads");
                auto thread_queue_size = tree.get<int>("resources.thread-pool.thread_queue_size");
                int available_cpu_cores = std::thread::hardware_concurrency() - 2;
                num_threads = available_cpu_cores < num_threads ? available_cpu_cores : num_threads;
                _thread_pool = make_shared<ctpl::thread_pool>(num_threads, thread_queue_size);
            }

            // feature operators
            {
                auto feat_ops_items = tree.get_child_optional("resources.feature-operators");
                if (feat_ops_items.is_initialized()) {
                    for (auto op_item_ = feat_ops_items.value().begin();
                         op_item_ != feat_ops_items.value().end(); ++op_item_) {
                        auto op_item = *op_item_;
                        if (op_item.first == "feat-op") {
                            auto op_name = op_item.second.get_optional<string>("op");
                            if (op_name.is_initialized()) {
                                create_base_feat_ops();
                            }
                        } else if (op_item.first != "<xmlcomment>") {
                            _logger->error("Parsed unknown feature operator: {}", op_item.first);
                            return;
                        }
                    }
                }
            }

            // attribute transformers
            {
                auto attr_transer_items = tree.get_child_optional("resources.attribute-transformers");
                if (attr_transer_items.is_initialized()) {
                    for (const pt::ptree::value_type &transer_item: attr_transer_items.value()) {
                        if (transer_item.first == "attr-transer") {
                            auto name = transer_item.second.get_optional<string>("name");
                            auto conf_fname = transer_item.second.get_optional<string>("conf_fname");
                            if (name.is_initialized() && conf_fname.is_initialized()) {
                                _attr_transers[name.value()] = make_shared<AttrTranser>(conf_fname.value(),
                                                                                        conf_dname,
                                                                                        this->_thread_pool,
                                                                                        thread_type);
                            } else {
                                _logger->error("Parsed no name or conf_name in attribute transformer.");
                                return;
                            }
                        } else if (transer_item.first != "<xmlcomment>") {
                            _logger->error("Parsed unknown attribute transformer: {}", transer_item.first);
                            return;
                        }
                    }
                }
            }

            // feature transformers
            {
                auto feat_transer_items = tree.get_child_optional("resources.feature-transformers");
                if (feat_transer_items.is_initialized()) {
                    for (const pt::ptree::value_type &transer_item: feat_transer_items.value()) {
                        if (transer_item.first == "feat-transer") {
                            auto name = transer_item.second.get_optional<string>("name");
                            auto conf_fname = transer_item.second.get_optional<string>("conf_fname");
                            if (name.is_initialized() && conf_fname.is_initialized()) {
                                _feat_transers[name.value()] = make_shared<FeatTranser>(conf_fname.value(),
                                                                                        conf_dname,
                                                                                        this->_thread_pool,
                                                                                        this->_feat_ops,
                                                                                        thread_type);
                            } else {
                                _logger->error("Parsed no name or conf_fname in feature transformer.");
                                return;
                            }
                        } else {
                            _logger->error("Parsed unknown feature transformer: {}", transer_item.first);
                            return;
                        }
                    }
                }
            }

            // feature encoders
            {
                auto feat_enc_items = tree.get_child_optional("resources.feature-encoders");
                if (feat_enc_items.is_initialized()) {
                    for (const pt::ptree::value_type &enc_item: feat_enc_items.value()) {
                        if (enc_item.first == "feat-hash") {
                            auto name = enc_item.second.get_optional<string>("name");
                            auto hash_bits_ = enc_item.second.get_optional<int>("hash-bits");
                            if (name.is_initialized() && hash_bits_.is_initialized()) {
                                auto hash_bits = hash_bits_.value();
                                if (hash_bits > 32 || hash_bits <= 10) {
                                    _logger->error(
                                            "Number of hash bits is greater than 32 and no less than 10, but got {}.",
                                            hash_bits);
                                    return;
                                }

                                _feat_encoders[name.value()] = make_shared<FeatureHash>(1 << hash_bits);
                            } else {
                                _logger->error("Parsed no name or hash size in feature hash.");
                                return;
                            }
                        } else if (enc_item.first == "feat-map") {
                            auto name = enc_item.second.get_optional<string>("name");
                            auto conf_fname = enc_item.second.get_optional<string>("conf_fname");
                            if (name.is_initialized() && conf_fname.is_initialized()) {
                                _feat_encoders[name.value()] = make_shared<FeatureMap>(
                                        conf_dname + "/" + conf_fname.value());
                            } else {
                                _logger->error("Parsed no name or conf_fname in feature map.");
                                return;
                            }
                        } else {
                            _logger->error("Parsed unknown feature encoder item: {}", enc_item.first);
                            return;
                        }
                    }
                }
            }

            // models
            {
                auto model_items = tree.get_child_optional("resources.models");
                if (model_items.is_initialized()) {
                    for (const pt::ptree::value_type &model_item: model_items.value()) {
                        if (model_item.first == "model") {
                            auto name_ = model_item.second.get_optional<string>("name");
                            auto type_ = model_item.second.get_optional<string>("type");
                            auto fname_ = model_item.second.get_optional<string>("fname");
                            auto update_interval_ = model_item.second.get_optional<int>("update-interval");

                            if (name_.is_initialized() && type_.is_initialized() && fname_.is_initialized() &&
                                update_interval_.is_initialized()) {
                                auto name = name_.value();
                                auto type = type_.value();
                                auto fname = fname_.value();
                                auto update_interval = update_interval_.value();

                                if (name == "" || type == "" || fname == "") {
                                    _logger->error("Parsed empty name or type or fname in model");
                                    return;
                                } else {
                                    if (type == "vw-model") {
                                    } else if (type == "skl-model") {
                                    } else if (type == "xgb-predictor") {
                                    } else if (type == "raw-gbdt-model") {
                                    } else if (type == "adaqs-model") {
                                    } else {
                                        _logger->error("Parsed unknown model type {}", type);
                                        return;
                                    }

                                    _logger->info("Will load model name={}, type={}, fname={}", name, type, fname);
                                    _models[name] = make_shared<ModelDaemon>(conf_dname + "/" + fname,
                                                                             type,
                                                                             update_interval);
                                }
                            } else {
                                _logger->error("Parsed no name or type or fname in model.");
                                return;
                            }

                        } else {
                            _logger->error("Parsed unknown model item: {}", model_item.first);
                            return;
                        }
                    }
                }
            }

            // calibrations
            {
                auto calibration_items = tree.get_child_optional("resources.calibrations");
                if (calibration_items.is_initialized()) {
                    for (const pt::ptree::value_type &calibration_item: calibration_items.value()) {
                        if (calibration_item.first == "calibration") {
                            auto name_ = calibration_item.second.get_optional<string>("name");
                            auto type_ = calibration_item.second.get_optional<string>("type");
                            auto fname_ = calibration_item.second.get_optional<string>("fname");
                            auto update_interval_ = calibration_item.second.get_optional<int>("update-interval");

                            if (name_.is_initialized() && type_.is_initialized() && fname_.is_initialized() &&
                                update_interval_.is_initialized()) {
                                auto name = name_.value();
                                auto type = type_.value();
                                auto fname = fname_.value();
                                auto update_interval = update_interval_.value();

                                if (name == "" || type == "" || fname == "") {
                                    _logger->error("Parsed empty name or type or fname in calibration");
                                    return;
                                } else {
                                    if (type == "isotonic-regression") {
                                    } else if (type == "prior-correction") {
                                    } else if (type == "hybrid") {
                                    } else {
                                        _logger->error("Parsed unknown calibration type {}", type);
                                        return;
                                    }

                                    _logger->info("Will load calibration name={}, type={}, fname={}", name, type,
                                                  fname);
                                    _calibrations[name] = make_shared<CalibrationDaemon>(conf_dname, fname, type,
                                                                                         update_interval);
                                }
                            } else {
                                _logger->error("Parsed no name or type or fname in calibration.");
                                return;
                            }

                        } else {
                            _logger->error("Parsed unknown calibration item: {}", calibration_item.first);
                            return;
                        }
                    }
                }
            }

            _started = true;
            _logger->info("Finish creating resources.");
            _logger->flush();

        } catch (const exception &ex) {
            _logger->error("Initialize resources fail.", ex.what());
        }

    }

    ResourceManager::~ResourceManager() {
        // release thread pool
        if (_thread_pool) {
            _thread_pool->stop(true);
            _thread_pool->clear_queue();
        }
        _started = false;
        _logger->info("Release resources.");
        _logger->flush();
    }
}