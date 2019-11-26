//
// Created by qfeng on 2019-01-16.
//

#include "feature/comp_op_ctx.h"
#include "feature/compare_op.h"
#include <absl/strings/str_split.h>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

namespace algocomp {

    bool CompOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "") {
                _logger->warn(
                        "Empty procs for compare feature operation context (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                        _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                return false;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error("Parsed inconsistent sizes of procs {} and params {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                               proc_desc, param_desc, _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);

                // Parse processes and their corresponding parameters
                if (proc_name == "all") {
                    _avg_dot = true;
                    _avg_max = true;
                    _avg_min = true;
                } else if (proc_name == "partial") {
                    std::vector<std::string> comp_params = absl::StrSplit(param_splits[i], ',');
                    for (int i = 0; i < comp_params.size(); ++i) {
                        auto param_name = trim_copy(comp_params[i]);

                        if (param_name == "avg_dot") {
                            _avg_dot = true;
                        } else if (param_name == "avg_max") {
                            _avg_max = true;
                        } else if (param_name == "avg_min") {
                            _avg_min = true;
                        } else {
                            _logger->error(
                                    "Parsed unknown compare param {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                                    comp_params[i], _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                            return false;
                        }
                    }
                } else {
                    _logger->error(
                            "Parsed unknown compare proc {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                            proc_desc, _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                    return false;
                }
            }

            return true;

        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong compare procs {} and params {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                    proc_desc, param_desc, _in_gname1, _in_fname1, _in_gname2, _in_fname2);
        }

        return false;
    }

    optional<pair<string, ContFeatGrp>>
    CompOpCtx::apply(const vector<google::protobuf::Map<string, algocomp::ContFeatGrp>> &inp_feats,
                     google::protobuf::Arena &arena) const {
        try {
            int g1_vec_idx = -1;
            int g2_vec_idx = -1;

            // Firstly record the indices of matched feature group
            for (int i = 0; i < inp_feats.size(); ++i) {
                auto inp_feats_ = inp_feats[i];
                auto g1_found = inp_feats_.find(_in_gname1);
                auto g2_found = inp_feats_.find(_in_gname2);

                if (g1_found != inp_feats_.end()) {
                    g1_vec_idx = i;
                }

                if (g2_found != inp_feats_.end()) {
                    g2_vec_idx = i;
                }
            }

            if (g1_vec_idx != -1 && g2_vec_idx != -1) {
                // Secondly find the matched features
                auto g1_found = inp_feats[g1_vec_idx].find(_in_gname1);
                auto g2_found = inp_feats[g2_vec_idx].find(_in_gname2);

                auto group1 = g1_found->second.feats();
                auto group2 = g2_found->second.feats();

                optional<pair<string, ContFeatGrp>> res = std::nullopt;

                if (_op_scope == OP_SCOPE::GNAME) {
                    // Apply the compare transform
                    res = _op->apply(group1, group2, *this, arena);
                } else if (_op_scope == OP_SCOPE::FNAME) {
                    auto f1_found = group1.find(_in_fname1);
                    auto f2_found = group2.find(_in_fname2);

                    if (f1_found != group1.end() && f2_found != group2.end()) {
                        auto feat1 = f1_found->second;
                        auto feat2 = f2_found->second;

                        // Apply the compare transform
                        res = _op->apply(feat1, feat2, *this, arena);
                    }
                }

                if (res.has_value() && res.value().second.feats_size() > 0) {
                    return res;
                } else {
                    _logger->warn("Compare features fail on feature {}/{} - {}/{}", _in_gname1,
                                  _in_fname1, _in_gname2, _in_fname2);
                }
            }

        } catch (const exception &ex) {
            _logger->error("Compare fail on features: {}", ex.what());
        }

        return std::nullopt;
    }
}