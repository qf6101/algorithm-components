//
// Created by qfeng on 2019-01-07.
//

#include "feature/cross_op_ctx.h"
#include "feature/cross_op.h"
#include "resource/resource_manager.h"

namespace algocomp {
    bool CrossOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        return true;
    }

    optional<pair<string, DiscFeatGrp>>
    CrossOpCtx::apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
                      google::protobuf::Arena &arena) const {
        try {
            DiscFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);

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

                if (_op_scope == OP_SCOPE::GNAME) {
                    for (auto &feat1_: group1) {
                        for (auto &feat2_: group2) {
                            auto feat1 = feat1_.second;
                            auto feat2 = feat2_.second;

                            // Apply the cross transform
                            auto res = _op->apply(feat1, feat2, *this, arena);

                            if (res.has_value()) {
                                auto &res_feat = res.value();
                                (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                            } else {
                                _logger->warn("Cross fail on feature (scope=gname) {}/{} - {}/{}", _in_gname1,
                                              feat1.fname(), _in_gname2, feat2.fname());
                            }
                        }
                    }
                } else if (_op_scope == OP_SCOPE::FNAME) {
                    auto f1_found = group1.find(_in_fname1);
                    auto f2_found = group2.find(_in_fname2);

                    if (f1_found != group1.end() && f2_found != group2.end()) {
                        auto feat1 = f1_found->second;
                        auto feat2 = f2_found->second;

                        // Apply the cross transform
                        auto res = _op->apply(feat1, feat2, *this, arena);

                        if (res.has_value()) {
                            auto &res_feat = res.value();
                            (*outp_feats->mutable_feats())[res_feat.fname()] = res_feat;
                        } else {
                            _logger->warn("Cross fail on feature (scope=fname) {}/{} - {}/{}", _in_gname1,
                                          _in_fname1, _in_gname2, _in_fname2);
                        }
                    }
                }
            }

            if (outp_feats->feats_size() > 0) {
                auto outp_gname = outp_feats->feats().begin()->second.gname();
                return make_pair(outp_gname, *outp_feats);
            }

        } catch (const exception &ex) {
            _logger->error("Cross fail on features: {}.", ex.what());
        }

        return std::nullopt;
    }

    optional<map<string, DiscFeatGrp>> CrossOpCtx::batch_apply(
            const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
            google::protobuf::Arena &arena) const {
        // Do not support transform for batch data
        return std::nullopt;
    }

}
