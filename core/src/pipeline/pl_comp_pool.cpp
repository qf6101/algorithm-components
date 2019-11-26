//
// Created by qfeng on 2018-12-29.
//

#include "pipeline/pl_comp_pool.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <boost/filesystem/path.hpp>
#include "log/algocomp_log.h"
#include "encode/feat_enc_daemon.h"

using namespace std;
namespace pt = boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace algocomp {

    PLCompPool::PLCompPool(const string &conf_fname) {
        _logger = spdlog::get(ACLog::_logger_name);

        try {
            pt::ptree tree;
            pt::read_xml(conf_fname, tree, trim_whitespace | no_comments);

            auto conf_dname = boost::filesystem::path(conf_fname).parent_path().string();

            ThreadType thread_type;
            // global param
            {
                auto global_params = tree.get_child_optional("pipeline-components.global-params");
                if (global_params.is_initialized()) {

                    for (auto param_item_ = global_params.value().begin();
                         param_item_ != global_params.value().end(); ++param_item_) {
                        auto param_item = *param_item_;

                        if (param_item.first == "thread-type") {
                            auto thread_type_ = param_item.second.data();

                            if (thread_type_ == "on-example") {
                                thread_type = ThreadType::ON_EXAMPLE;
                            } else if (thread_type_ == "on-ads") {
                                thread_type = ThreadType::ON_ADS;
                            } else if (thread_type_ == "no") {
                                thread_type = ThreadType::NO;
                            } else {
                                _logger->error("Parsed unknown thread type {}", thread_type_);
                                return;
                            }
                        }
                    }
                } else {
                    _logger->error("Miss global params part in pipeline configuration which is a must part.");
                    return;
                }
            }

            // resource
            {
                auto rsrc_conf_fname = tree.get_optional<string>("pipeline-components.resource.conf-fname");
                if (rsrc_conf_fname.is_initialized()) {
                    _rsrc_mgr = make_unique<ResourceManager>(rsrc_conf_fname.value(), conf_dname, thread_type);
                } else {
                    _logger->error("Miss resource part in pipeline configuration which is a must part.");
                    return;
                }
            }

            // pl head
            {
                auto pl_head_items = tree.get_child_optional("pipeline-components.pipeline-heads");
                if (pl_head_items.is_initialized()) {
                    for (const pt::ptree::value_type &plh_item_: pl_head_items.value()) {
                        if (plh_item_.first == "pl-head") {
                            string name = "";
                            shared_ptr<AttrTranser> attr_transer;
                            vector<shared_ptr<FeatTranser>> feat_transers;

                            for (const pt::ptree::value_type &plh_item: plh_item_.second.get_child("")) {

                                if (plh_item.first == "name") {
                                    name = plh_item.second.data();

                                    if (name == "") {
                                        _logger->error("Parsed unnamed pl-head.");
                                        return;
                                    }

                                } else if (plh_item.first == "attr-transer") {
                                    auto at_name = plh_item.second.data();
                                    attr_transer = _rsrc_mgr->get_attr_transer(at_name);
                                    if (attr_transer == nullptr) {
                                        _logger->error("Parsed unknown attribute transformer: {}", at_name);
                                        return;
                                    }
                                } else if (plh_item.first == "feat-transer") {
                                    auto ft_name = plh_item.second.data();
                                    auto feat_transer = _rsrc_mgr->get_feat_transer(ft_name);
                                    if (feat_transer != nullptr) {
                                        feat_transers.emplace_back(feat_transer);
                                    } else {
                                        _logger->error("Parsed unknown feature transformer: {}", ft_name);
                                        return;
                                    }
                                } else if (plh_item.first != "<xmlcomment>") {
                                    _logger->error("Parsed unknown pl-head item: {}", plh_item.first);
                                    return;
                                }

                            }

                            _pl_heads[name] = make_shared<PLHead>(attr_transer, feat_transers, _rsrc_mgr->_thread_pool,
                                                                  thread_type);
                        }
                    }
                }
            }

            // pl middle
            {
                auto pl_middle_items = tree.get_child_optional("pipeline-components.pipeline-middles");
                if (pl_middle_items.is_initialized()) {
                    for (const pt::ptree::value_type &plm_item_: pl_middle_items.value()) {
                        if (plm_item_.first == "pl-middle") {
                            string name = "";
                            vector<shared_ptr<FeatTranser>> feat_transers;
                            shared_ptr<FeatEncDaemon> feat_enc_daemon;

                            for (const pt::ptree::value_type &plm_item: plm_item_.second.get_child("")) {
                                if (plm_item.first == "name") {
                                    name = plm_item.second.data();

                                    if (name == "") {
                                        _logger->error("Parsed unnamed pl-middle.");
                                        return;
                                    }

                                } else if (plm_item.first == "feat-transer") {
                                    auto ft_name = plm_item.second.data();
                                    auto feat_transer = _rsrc_mgr->get_feat_transer(ft_name);
                                    if (feat_transer != nullptr) {
                                        feat_transers.emplace_back(feat_transer);
                                    } else {
                                        _logger->error("Parsed unknown feature transformer: {}", ft_name);
                                        return;
                                    }
                                } else if (plm_item.first == "feat-enc") {
                                    string feat_enc_name;
                                    string feat_enc_scope;
                                    string feat_enc_mode;
                                    string feat_enc_str_format;

                                    for (const pt::ptree::value_type &feat_enc_item: plm_item.second.get_child("")) {
                                        if (feat_enc_item.first == "feat-enc-name") {
                                            feat_enc_name = feat_enc_item.second.data();
                                        } else if (feat_enc_item.first == "feat-enc-scope") {
                                            feat_enc_scope = feat_enc_item.second.data();
                                        } else if (feat_enc_item.first == "feat-enc-mode") {
                                            feat_enc_mode = feat_enc_item.second.data();
                                        } else if (feat_enc_item.first == "feat-enc-str-format") {
                                            feat_enc_str_format = feat_enc_item.second.data();
                                        } else {
                                            _logger->error("Parsed unknown feature encode params {}.",
                                                           feat_enc_item.first);
                                        }
                                    }

                                    if (feat_enc_name != "" && feat_enc_scope != "" && feat_enc_mode != "" &&
                                        feat_enc_str_format != "") {
                                        auto feat_enc = _rsrc_mgr->get_feat_encoder(feat_enc_name);
                                        if (feat_enc == nullptr) {
                                            _logger->error("Parsed unknown feature encoder {}", feat_enc_name);
                                            return;
                                        }

                                        FeatEncScope fes;
                                        if (feat_enc_scope == "all") {
                                            fes = FeatEncScope::ALL;
                                        } else if (feat_enc_scope == "disc") {
                                            fes = FeatEncScope::DISC;
                                        } else if (feat_enc_scope == "cont") {
                                            fes = FeatEncScope::CONT;
                                        } else {
                                            _logger->error("Parsed unknown feature encode scope {}", feat_enc_scope);
                                            return;
                                        }

                                        FeatEncMode fem;
                                        if (feat_enc_mode == "accumulating") {
                                            fem = FeatEncMode::ACCUMULATING;
                                        } else if (feat_enc_mode == "processing") {
                                            fem = FeatEncMode::PROCESSING;
                                        } else {
                                            _logger->error("Parsed unknown feature encode mode {}", feat_enc_mode);
                                            return;
                                        }

                                        FeatEncStrFormat fesf;
                                        if (feat_enc_str_format == "vw") {
                                            fesf = FeatEncStrFormat::VW;
                                        } else if (feat_enc_str_format == "libsvm") {
                                            fesf = FeatEncStrFormat::LIBSVM;
                                        } else {
                                            _logger->error("Parsed unknown feature encode string format {}",
                                                           feat_enc_str_format);
                                            return;
                                        }

                                        feat_enc_daemon = make_shared<FeatEncDaemon>(feat_enc, fes, fem, fesf,
                                                                                     _rsrc_mgr->_thread_pool,
                                                                                     thread_type);
                                    } else {
                                        _logger->error("Miss one of feature encoder name, scope, mode or tostr.");
                                    }
                                } else if (plm_item.first != "<xmlcomment>") {
                                    _logger->error("Parsed unknown pl-middle item: {}", plm_item.first);
                                    return;
                                }
                            }

                            _pl_middles[name] = make_shared<PLMiddle>(feat_transers, _rsrc_mgr->_thread_pool,
                                                                      feat_enc_daemon, thread_type);

                        }
                    }
                }
            }

            // pl tail
            {
                auto pl_tail_items = tree.get_child_optional("pipeline-components.pipeline-tails");
                if (pl_tail_items.is_initialized()) {
                    for (const pt::ptree::value_type &plt_item_: pl_tail_items.value()) {
                        if (plt_item_.first == "pl-tail") {
                            string name = "";
                            shared_ptr<ModelDaemon> model_daemon;

                            for (const pt::ptree::value_type &plt_item: plt_item_.second.get_child("")) {
                                if (plt_item.first == "name") {
                                    name = plt_item.second.data();

                                    if (name == "") {
                                        _logger->error("Parsed unnamed pl-tail.");
                                        return;
                                    }
                                } else if (plt_item.first == "model") {
                                    auto model_name = plt_item.second.data();
                                    model_daemon = _rsrc_mgr->get_model(model_name);
                                    if (model_daemon == nullptr) {
                                        _logger->error("Parsed unknown model : {}", model_name);
                                        return;
                                    }
                                } else if (plt_item.first != "<xmlcomment>") {
                                    _logger->error("Parsed unknown pl-tail item: {}", plt_item.first);
                                    return;
                                }
                            }

                            _pl_tails[name] = make_shared<PLTail>(model_daemon, _rsrc_mgr->_thread_pool, thread_type);

                        }
                    }
                }
            }

            // pl calibration
            {
                auto pl_calibration_items = tree.get_child_optional("pipeline-components.pipeline-calibrations");
                if (pl_calibration_items.is_initialized()) {
                    for (const pt::ptree::value_type &plc_item_: pl_calibration_items.value()) {
                        if (plc_item_.first == "pl-calibration") {
                            string name = "";

                            // default null
                            shared_ptr<CalibrationDaemon> calibration_daemon = nullptr;

                            for (const pt::ptree::value_type &plc_item: plc_item_.second.get_child("")) {
                                if (plc_item.first == "name") {
                                    name = plc_item.second.data();

                                    if (name == "") {
                                        _logger->error("Parsed unnamed pl-calibration.");
                                        return;
                                    }

                                } else if (plc_item.first == "calibration") {
                                    auto calibration_name = plc_item.second.data();
                                    calibration_daemon = _rsrc_mgr->get_calibration(calibration_name);
                                    if (calibration_daemon == nullptr) {
                                        _logger->error("Parsed unknown calibration : {}", calibration_name);
                                        return;
                                    }

                                } else if (plc_item.first != "<xmlcomment>") {
                                    _logger->error("Parsed unknown pl-tail item: {}", plc_item.first);
                                    return;
                                }
                            }

                            _pl_calibrations[name] = calibration_daemon;
                        }
                    }
                }
            }

            // check zero pipeline or pipeline component
            if (_pl_heads.size() == 0 && _pl_middles.size() == 0 && _pl_tails.size() == 0) {
                _logger->error("Zero pipeline component.");
                return;
            }

            // pipeline
            {
                auto pl_items = tree.get_child_optional("pipeline-components.pipelines");
                if (pl_items.is_initialized()) {
                    for (const pt::ptree::value_type &pl_item_: pl_items.value()) {
                        string name = "";
                        shared_ptr<PLHead> plh = nullptr;
                        shared_ptr<PLMiddle> plm = nullptr;
                        shared_ptr<PLTail> plt = nullptr;

                        if (pl_item_.first == "pl") {
                            for (const pt::ptree::value_type &pl_item: pl_item_.second.get_child("")) {
                                if (pl_item.first == "name") {
                                    name = pl_item.second.data();

                                    if (name == "") {
                                        _logger->error("Parsed unnamed pipeline.");
                                        return;
                                    }
                                } else if (pl_item.first == "pl-head") {
                                    auto plh_ = pl_item.second.data();

                                    auto plh_found = _pl_heads.find(plh_);
                                    if (plh_found != _pl_heads.end()) {
                                        plh = std::make_shared<PLHead>(*plh_found->second);
                                    } else {
                                        _logger->error("Parsed unknown pipeline head {}", plh_);
                                        return;
                                    }
                                } else if (pl_item.first == "pl-middle") {
                                    auto plm_ = pl_item.second.data();

                                    auto plm_found = _pl_middles.find(plm_);
                                    if (plm_found != _pl_middles.end()) {
                                        plm = std::make_shared<PLMiddle>(*plm_found->second);
                                    } else {
                                        _logger->error("Parsed unknown pipeline middle {}", plm_);
                                        return;
                                    }
                                } else if (pl_item.first == "pl-tail") {
                                    auto plt_ = pl_item.second.data();

                                    auto plt_found = _pl_tails.find(plt_);
                                    if (plt_found != _pl_tails.end()) {
                                        plt = std::make_shared<PLTail>(*plt_found->second);
                                    } else {
                                        _logger->error("Parsed unknown pipeline tail {}", plt_);
                                        return;
                                    }
                                } else {
                                    _logger->error("Parsed unknown pipeline component item {}", pl_item.first);
                                }
                            }
                        } else {
                            _logger->error("Parsed unknown pipeline item {}", pl_item_.first);
                        }

                        int pl_comp_count = 0;
                        if (plh) pl_comp_count += 1;
                        if (plm) pl_comp_count += 1;
                        if (plt) pl_comp_count += 1;

                        if (pl_comp_count < 2) {
                            _logger->error("Pipeline should have more than 2 pl components, bug get {}", pl_comp_count);
                            return;
                        } else {
                            _pls[name] = make_shared<Pipeline>(plh, plm, plt, _rsrc_mgr->_thread_pool, thread_type);
                        }
                    }
                }
            }

            _logger->info("Finish creating pipeline component pool.");
            _logger->flush();

        } catch (const exception &ex) {
            _logger->error("Initialize pipeline components fail: {}", ex.what());
        }
    }

    PLHead *PLCompPool::get_plhead(const string &name) {
        if (_pl_heads.find(name) != _pl_heads.end()) {
            return _pl_heads[name].get();
        } else {
            _logger->error("No matched pl head found: {}", name);
            return nullptr;
        };
    }

    shared_ptr<CalibrationDaemon> PLCompPool::get_calibration(const string &name) {
        if (_pl_calibrations.find(name) != _pl_calibrations.end()) {
            return _pl_calibrations[name];
        } else if (_pl_calibrations.find("default") != _pl_calibrations.end()) {
            return _pl_calibrations["default"];
        } else {
            _logger->error("No matched pl calibration found: {}", name);
            return nullptr;
        };
    }

    PLMiddle *PLCompPool::get_plmiddle(const string &name) {
        if (_pl_middles.find(name) != _pl_middles.end()) {
            return _pl_middles[name].get();
        } else {
            _logger->error("No matched pl middle found: {}", name);
            return nullptr;
        }
    }

    PLTail *PLCompPool::get_pltail(const string &name) {
        if (_pl_tails.find(name) != _pl_tails.end()) {
            return _pl_tails[name].get();
        } else {
            _logger->error("No matched pl tail found: {}", name);
            return nullptr;
        }
    }

    Pipeline *PLCompPool::get_pipeline(const string &name) {
        if (_pls.find(name) != _pls.end()) {
            return _pls[name].get();
        } else {
            _logger->error("No matched pipeline found: {}", name);
            return nullptr;
        }
    }

    vector<float> PLCompPool::calibrate(const vector<string> &pids, const vector<float> &scores) {
        vector<float> new_scores;
        if (pids.size() == scores.size() && pids.size() != 0) {
            new_scores.reserve(scores.size());
            for (int i = 0; i < scores.size(); ++i) {
                auto plc = get_calibration(pids[i]);
                auto new_score = plc->calibrate(scores[i]);
                new_scores.push_back(new_score);
            }
        } else {
            _logger->error("Inconsistent size of input pids and scores or zero input (ps={}, ss={}).", pids.size(),
                           scores.size());
        }
        return new_scores;
    }

    float PLCompPool::calibrate(const string &pid, float score) {
        auto plc = get_calibration(pid);
        auto new_score = plc->calibrate(score);
        return new_score;
    }
};
