//
// Created by qfeng on 2019-01-19.
//

#include <feature/feat_op_ctx_pack.h>
#include <absl/strings/str_cat.h>

namespace algocomp {

    bool FeatOpCtxPack::self_load() {
        try {
            // Initialize index map of ccu feature operation context
            for (int i = 0; i < ccu_ctx.size(); ++i) {
                ccu_idx[ccu_ctx[i]->_in_gname] = i;
            }

            // Initialize index map of ccb feature operation context
            for (int i = 0; i < ccb_ctx.size(); ++i) {
                auto gname1 = ccb_ctx[i]->_in_gname1;
                auto gname2 = ccb_ctx[i]->_in_gname2;

                auto g1_found = ccb_idx.find(gname1);
                if (g1_found != ccb_idx.end()) {
                    g1_found->second.insert(i);
                } else {
                    ccb_idx[gname1] = set<int>{i};
                }

                auto g2_found = ccb_idx.find(gname2);
                if (g2_found != ccb_idx.end()) {
                    g2_found->second.insert(i);
                } else {
                    ccb_idx[gname2] = set<int>{i};
                }
            }

            // Initialize index map of cdu feature operation context
            for (int i = 0; i < cdu_ctx.size(); ++i) {
                cdu_idx[cdu_ctx[i]->_in_gname] = i;
            }

            // Initialize index map of ddb feature operation context
            for (int i = 0; i < ddb_ctx.size(); ++i) {

                if (ddb_ctx[i]->batch()) {
                    _batch_ddb_ctx.push_back(i);
                } else {
                    auto gname1 = ddb_ctx[i]->_in_gname1;
                    auto gname2 = ddb_ctx[i]->_in_gname2;

                    auto g1_found = ddb_idx.find(gname1);
                    if (g1_found != ddb_idx.end()) {
                        g1_found->second.insert(i);
                    } else {
                        ddb_idx[gname1] = set<int>{i};
                    }

                    auto g2_found = ddb_idx.find(gname2);
                    if (g2_found != ddb_idx.end()) {
                        g2_found->second.insert(i);
                    } else {
                        ddb_idx[gname2] = set<int>{i};
                    }
                }
            }

            return true;
        } catch (const exception &ex) {
            _logger->error("Load feature opctx pack fail: {}", ex.what());
        }

        return false;
    }

    const vector<int>& FeatOpCtxPack::batch_ddb_ctx() const {
        return _batch_ddb_ctx;
    }

    vector<int> FeatOpCtxPack::find_in_ccu(
            const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const {
        vector<int> outp;
        outp.reserve(inp_feats.size());

        for (auto &feats: inp_feats) {
            auto found = ccu_idx.find(feats.first);
            if (found != ccu_idx.end()) {
                outp.push_back(found->second);
            }
        }
        return outp;
    }

    vector<int> FeatOpCtxPack::find_in_ccb(
            const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const {
        set<int> outp_;

        for (auto &feats: inp_feats) {
            auto found = ccb_idx.find(feats.first);
            if (found != ccb_idx.end()) {
                outp_.insert(found->second.begin(), found->second.end());
            }
        }

        std::vector<int> outp(outp_.begin(), outp_.end());
        return outp;
    }

    vector<int> FeatOpCtxPack::find_in_cdu(
            const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const {
        vector<int> outp;
        outp.reserve(inp_feats.size());

        for (auto &feats: inp_feats) {
            auto found = cdu_idx.find(feats.first);
            if (found != cdu_idx.end()) {
                outp.push_back(found->second);
            }
        }

        return outp;
    }

    vector<int> FeatOpCtxPack::find_in_ddb(
            const ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp> &inp_feats) const {
        set<int> outp_;

        for (auto &feats: inp_feats) {

            auto found = ddb_idx.find(feats.first);
            if (found != ddb_idx.end()) {
                outp_.insert(found->second.begin(), found->second.end());
            }
        }

        std::vector<int> outp(outp_.begin(), outp_.end());
        return outp;
    }
}