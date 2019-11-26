//
// Created by qfeng on 2019-01-04.
//

#include "feature/bucketize_op_ctx.h"
#include "feature/bucketize_op.h"
#include "resource/resource_manager.h"
#include <google/protobuf/arena.h>
#include <absl/strings/str_split.h>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;


namespace algocomp {
    vector<float> BucketizeOpCtx::_ten_equal_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1};

    // -1.645 ~ 1.645 wrt lower_p=0.95
    vector<float> BucketizeOpCtx::_ten_equal_width_zscore = {-0.165, -0.132, -0.099, -0.066, -0.033, 0., 0.033, 0.066,
                                                             0.099, 0.132, 0.165};


    bool BucketizeOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "" && param_desc == "") {
                _logger->warn(
                        "Empty procs and params for bucketize feature operation context (in_gname={}, in_fname={}).",
                        _in_gname, _in_fname);
                return false;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error(
                        "Parsed inconsistent sizes of procs {} and params {} (in_gname={}, in_fname={}).",
                        proc_desc, param_desc, _in_gname, _in_fname);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);

                if (proc_name == "customized") { // Parse process of customized boundaries
                    _boundaries.clear();
                    std::vector<std::string> c_splits = absl::StrSplit(param_splits[i], ',');
                    for (auto &s: c_splits) {
                        _boundaries.emplace_back(std::move(std::stof(s)));
                    }
                } else if (proc_name == "predefined") { // Parse process of predefined boundaries
                    if (trim_copy(param_splits[i]) == "ten-equal-width") {
                        _boundaries = BucketizeOpCtx::_ten_equal_width;
                    } else if (trim_copy(param_splits[i]) == "ten-equal-width-zscore") {
                        _boundaries = BucketizeOpCtx::_ten_equal_width_zscore;
                    } else {
                        _logger->error("Parsed unknown bucketize param {} (in_gname={}, in_fname={}).",
                                       param_splits[i],
                                       _in_gname, _in_fname);
                        return false;
                    }
                } else {
                    _logger->error("Parsed unknown bucketize proc {} (in_gname={}, in_fname={}).",
                                   proc_splits[i],
                                   _in_gname, _in_fname);
                    return false;
                }
            }

            return true;

        } catch (const std::exception &ex) {
            _logger->error("Parsed wrong bucketize procs {} and params {} (in_gname={}, in_fname={}).",
                           proc_desc,
                           param_desc, _in_gname, _in_fname);
        }

        return false;
    }

    const vector<float> BucketizeOpCtx::boundaries() const {
        return _boundaries;
    }

    optional<pair<string, DiscFeatGrp>>
    BucketizeOpCtx::apply(const vector<google::protobuf::Map<string, algocomp::ContFeatGrp>> &inp_feats,
                          google::protobuf::Arena &arena) const {
        try {
            DiscFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);

            for (auto &inp_feats_: inp_feats) {
                // Match input feature
                auto g_found = inp_feats_.find(_in_gname);
                if (g_found != inp_feats_.end()) {
                    auto &feats = (*g_found).second.feats();

                    if (this->op_scope() == OP_SCOPE::GNAME) {
                        for (auto &feat: feats) {
                            // Apply bucketizing transform on every feature of matched feature group name
                            auto res = this->_op->apply(feat.second, *this, arena);
                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                            } else {
                                _logger->warn("Bucketize fail on feature {} - {}", _in_gname, feat.first);
                            }
                        }
                    } else if (this->op_scope() == OP_SCOPE::FNAME) {
                        auto f_found = feats.find(_in_fname);
                        if (f_found != feats.end()) {
                            // Apply bucketizing transform on matched feature
                            auto &feat = (*f_found).second;
                            auto res = this->_op->apply(feat, *this, arena);

                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                            } else {
                                _logger->warn("Bucketize fail on feature {} - {}", _in_gname, feat.fname());
                            }
                        }
                    }
                }
            }

            if (outp_feats->feats_size() > 0) {
                auto outp_gname = outp_feats->feats().begin()->second.gname();
                return make_pair(outp_gname, *outp_feats);
            } else {
                return std::nullopt;
            }
        } catch (const exception &ex) {
            _logger->error("Bucketize fail on features: {}.", ex.what());
        }

        return std::nullopt;
    }
}
