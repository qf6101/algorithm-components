//
// Created by qfeng on 2019-01-08.
//

#include <feature/log_op_ctx.h>

#include "feature/log_op_ctx.h"
#include "featpb/cont_feat_grp.pb.h"
#include "feature/log_op.h"
#include <string>
#include <optional>
#include <absl/strings/str_split.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

namespace algocomp {
    bool LogOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "") {
                _logger->warn(
                        "Empty procs for log feature operation context (in_gname={}, in_fname={}).",
                        _in_gname, _in_fname);
                return false;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error("Parsed inconsistent sizes of procs {} and params {} (in_gname={}, in_fname={}).",
                               proc_desc, param_desc, _in_gname, _in_fname);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);

                // Parse processes and their corresponding parameters
                if (proc_name == "log10") {
                    _log_op_type = LogOpType::LOG10;
                } else if (proc_name == "log") {
                    _log_op_type = LogOpType::LOG;
                } else if (proc_name == "log2") {
                    _log_op_type = LogOpType::LOG2;
                } else {
                    _logger->error("Parsed unknown log proc {} (in_gname={}, in_fname={})", proc_splits[i], _in_gname,
                                   _in_fname);
                    return false;
                }
            }

            return true;

        } catch (const std::exception &ex) {
            _logger->error("Parsed wrong log procs {} and params {} (in_gname={}, in_fname={}).", proc_desc, param_desc,
                           _in_gname, _in_fname);
        }

        return false;
    }

    optional<pair<string, ContFeatGrp>>
    LogOpCtx::apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
                    google::protobuf::Arena &arena) const {
        try {
            ContFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<ContFeatGrp>(&arena);

            for (auto &inp_feats_: inp_feats) {
                // Match input feature
                auto g_found = inp_feats_.find(_in_gname);
                if (g_found != inp_feats_.end()) {
                    auto &feats = (*g_found).second.feats();

                    if (this->op_scope() == OP_SCOPE::GNAME) {
                        for (auto &feat: feats) {
                            // Apply logarithm transform on every feature of matched feature group name
                            auto res = this->_op->apply(feat.second, *this, arena);
                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                            } else {
                                _logger->warn("Logarithm (gname scope) fail on feature {} - {}", _in_gname, feat.first);
                            }
                        }
                    } else if (this->op_scope() == OP_SCOPE::FNAME) {
                        auto f_found = feats.find(_in_fname);
                        if (f_found != feats.end()) {
                            auto &feat = (*f_found).second;
                            // Apply logarithm transform on matched feature name
                            auto res = this->_op->apply(feat, *this, arena);

                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                            } else {
                                _logger->warn("Logarithm (fname scope) fail on feature {} - {}", _in_gname, feat.fname());
                            }
                        }
                    }
                }
            }

            if (outp_feats->feats_size() > 0) {
                auto outp_gname = outp_feats->feats().begin()->second.gname();
                return make_pair(outp_gname, *outp_feats);
            } else return std::nullopt;

        } catch (const exception &ex) {
            _logger->error("Log wised fail on features: {}", ex.what());
        }

        return std::nullopt;
    }
}
