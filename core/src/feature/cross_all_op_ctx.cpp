//
// Created by qfeng on 2019-01-18.
//

#include "feature/cross_all_op_ctx.h"
#include <absl/strings/str_split.h>
#include <boost/algorithm/string.hpp>
#include "absl/strings/str_cat.h"
#include "feature/cross_op.h"
#include "absl/strings/match.h"

using namespace boost::algorithm;

namespace algocomp {

    bool CrossAllOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "") {
                _logger->warn(
                        "Empty procs for cross-all feature operation context (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                        _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                return false;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error(
                        "Parsed inconsistent sizes of procs {} and params {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                        proc_desc, param_desc, _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);

                // Parse process and parameters for cross-all transform
                if (proc_name == "uc-inner") {
                    _cross_all_op_type = CrossAllOpType::UC_INNER;
                } else if (proc_name == "ad-inner") {
                    _cross_all_op_type = CrossAllOpType::AD_INNER;
                } else if (proc_name == "ad2uc") {
                    _cross_all_op_type = CrossAllOpType::AD2UC;
                } else if (proc_name == "ignore_uc_gnames") {
                    std::vector<std::string> ignore_params = absl::StrSplit(param_splits[i], ',');
                    for (auto &ignore_gname: ignore_params) _ignore_uc_gnames.insert(ignore_gname);
                } else if (proc_name == "ignore_ad_gnames") {
                    std::vector<std::string> ignore_params = absl::StrSplit(param_splits[i], ',');
                    for (auto &ignore_gname: ignore_params) _ignore_ad_gnames.insert(ignore_gname);
                } else if (proc_name == "ignore_uc_fnames") {
                    std::vector<std::string> ignore_params = absl::StrSplit(param_splits[i], ',');
                    for (auto &ignore_fname: ignore_params) _ignore_uc_fnames.insert(ignore_fname);
                } else if (proc_name == "ignore_ad_fnames") {
                    std::vector<std::string> ignore_params = absl::StrSplit(param_splits[i], ',');
                    for (auto &ignore_fname: ignore_params) _ignore_ad_fnames.insert(ignore_fname);
                } else if (proc_name == "ignore_double_cross") {
                    if (absl::AsciiStrToLower(param_splits[i]) == "true") _ignore_double_cross = true;
                } else {
                    _logger->error(
                            "Parsed unknown cross-all proc {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                            proc_splits[i], _in_gname1, _in_fname1, _in_gname2, _in_fname2);
                    return false;
                }
            }

            return true;
        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong cross-all procs {} and params {} (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}).",
                    proc_desc, param_desc, _in_gname1, _in_fname1, _in_gname2, _in_fname2);
        }

        return false;
    }

    optional<pair<string, DiscFeatGrp>> CrossAllOpCtx::apply(
            const vector<google::protobuf::Map<string, algocomp::DiscFeatGrp>> &inp_feats,
            google::protobuf::Arena &arena) const {
        // Do not support transform for single data
        return std::nullopt;
    }

    optional<map<string, DiscFeatGrp>> CrossAllOpCtx::batch_apply(
            const vector<google::protobuf::Map<string, algocomp::DiscFeatGrp>> &inp_feats,
            google::protobuf::Arena &arena) const {

        if (inp_feats.size() == 0) {
            _logger->warn("Empty input features for cross all transform.");
            return std::nullopt;
        }

        if (_cross_all_op_type == CrossAllOpType::UC_INNER) {
            if (inp_feats.size() == 0) {
                _logger->warn("Incorrect uc inner cross");
                return std::nullopt;
            } else
                return impl_crossing(inp_feats[0], inp_feats[0], _ignore_uc_gnames, _ignore_uc_fnames,
                                     _ignore_uc_gnames, _ignore_uc_fnames, arena, "uc");
        }

        if (_cross_all_op_type == CrossAllOpType::AD_INNER) {
            if (inp_feats.size() <= 1) {
                _logger->warn("Incorrect ad inner cross.");
                return std::nullopt;
            } else
                return impl_crossing(inp_feats[1], inp_feats[1], _ignore_ad_gnames, _ignore_ad_fnames,
                                     _ignore_ad_gnames, _ignore_ad_fnames, arena, "ad");
        }

        if (_cross_all_op_type == CrossAllOpType::AD2UC) {
            if (inp_feats.size() <= 1) {
                _logger->warn("Incorrect ad2uc cross.");
                return std::nullopt;
            } else
                return impl_crossing(inp_feats[1], inp_feats[0], _ignore_ad_gnames, _ignore_ad_fnames,
                                     _ignore_uc_gnames, _ignore_uc_fnames, arena, "ad2uc");
        }
    }

    optional<map<string, DiscFeatGrp>>
    CrossAllOpCtx::impl_crossing(const google::protobuf::Map<string, algocomp::DiscFeatGrp> &inp_lhs_feats,
                                 const google::protobuf::Map<string, algocomp::DiscFeatGrp> &inp_rhs_feats,
                                 const google::dense_hash_set<string> &ignore_lhs_gnames,
                                 const google::dense_hash_set<string> &ignore_lhs_fnames,
                                 const google::dense_hash_set<string> &ignore_rhs_gnames,
                                 const google::dense_hash_set<string> &ignore_rhs_fnames,
                                 google::protobuf::Arena &arena,
                                 const string &how_cross) const {
        try {
            if (inp_lhs_feats.size() == 0 || inp_rhs_feats.size() == 0) {
                if (inp_lhs_feats.size() == 0) {
                    _logger->warn("Empty input lhs features for cross all transform.");
                }

                if (inp_rhs_feats.size() == 0) {
                    _logger->warn("Empty input rhs features for cross all transform.");
                }
                return std::nullopt;
            }

            set<string> already_ins;
            map<string, DiscFeatGrp> outp_res;

            for (auto &lhs: inp_lhs_feats) {
                // Skip the ignoring feature group names
                if (ignore_lhs_gnames.find(lhs.first) != ignore_lhs_gnames.end()) continue;

                for (auto &rhs: inp_rhs_feats) {
                    // Skip the ignoring feature group names
                    if (ignore_rhs_gnames.find(rhs.first) != ignore_rhs_gnames.end()) continue;

                    // Skip the cross-all between the same feature group
                    if (lhs.first == rhs.first) continue;

                    // Skip the already crossed features
                    string in_key;
                    if (lhs.first.compare(rhs.first) < 0) {
                        in_key = absl::StrCat(lhs.first, ";", rhs.first);
                    } else {
                        in_key = absl::StrCat(rhs.first, ";", lhs.first);
                    }

                    if (already_ins.find(in_key) != already_ins.end()) continue;
                    else {
                        already_ins.insert(in_key);
                    }

                    DiscFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                    string gname = "";

                    for (auto &lhs_elem: lhs.second.feats()) {
                        // Skip the ignoring feature names
                        if (ignore_lhs_fnames.find(lhs_elem.first) != ignore_lhs_fnames.end()) continue;

                        for (auto &rhs_elem: rhs.second.feats()) {
                            // Skip the ignoring feature names
                            if (ignore_rhs_fnames.find(rhs_elem.first) != ignore_rhs_fnames.end()) continue;

                            // Skip the cross-all between the same feature
                            if (lhs_elem.first == rhs_elem.first) continue;

                            auto lhs_data = lhs_elem.second;
                            auto rhs_data = rhs_elem.second;

                            // Apply the cross-all transform
                            auto res = _op->apply(lhs_data, rhs_data, *this, arena, how_cross, _ignore_double_cross);
                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                                gname = res_feat.gname();
                            } else if (!absl::StartsWith(lhs_data.gname(), "!cross") &&
                                       !absl::StartsWith(rhs_data.gname(), "!cross")) {
                                _logger->warn("Cross all fail on feature (scope=gname) {}/{} - {}/{}", lhs_data.gname(),
                                              lhs_data.fname(), rhs_data.gname(), rhs_data.fname());
                            }
                        }
                    }
                    if (gname != "" && outp_feats->feats_size() > 0) {
                        outp_res[gname] = *outp_feats;
                    }
                }
            }
            if (outp_res.size() > 0) {
                return outp_res;
            }
        } catch (const exception &ex) {
            _logger->error("Cross all fail on features: {}", ex.what());
        }

        return std::nullopt;
    }

}