//
// Created by qfeng on 2019-01-04.
//

#ifndef ALGORITHM_COMPONENTS_FEATOPCTXPACK_H
#define ALGORITHM_COMPONENTS_FEATOPCTXPACK_H

#include <vector>
#include "feature/feat_op_ctx.h"
#include "dense_hash_map"

using namespace std;

namespace algocomp {

    /**
     * Package of feature operation contexts
     *
     * The container of feature operation contexts for either user-and-context or ad.
     */
    class FeatOpCtxPack {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Indices of binary feature operation context from discrete features to discrete features with batch ability
        vector<int> _batch_ddb_ctx;

    public:
        // Constructor with logger and indices of feature operation context
        FeatOpCtxPack() {
            _logger = spdlog::get(ACLog::_logger_name);
            ccu_idx.set_empty_key("");
            ccb_idx.set_empty_key("");
            cdu_idx.set_empty_key("");
            ddb_idx.set_empty_key("");
        }

        // Feature operation context, the order is important when using these contexts!
        vector<shared_ptr<Cont2ContUnaryOpCtx>> ccu_ctx; // cont inp + cont outp + unary op
        vector<shared_ptr<Cont2ContBinaryOpCtx>> ccb_ctx; // cont inp + cont outp + binary op
        vector<shared_ptr<Cont2DiscUnaryOpCtx>> cdu_ctx; // cont inp + disc outp + unary op
        vector<shared_ptr<Disc2DiscBinaryOpCtx>> ddb_ctx; // disc inp + disc outp + binary op

        // Index map of feature operation contexts (used for fast matching with input feature group names)
        // Binary operations match with either gname1 or gname2. Hence inversely, one group feature name maybe match
        // more than one feature operation contexts
        google::dense_hash_map<string, int> ccu_idx;
        google::dense_hash_map<string, set<int>> ccb_idx;
        google::dense_hash_map<string, int> cdu_idx;
        google::dense_hash_map<string, set<int>> ddb_idx;

        // Get indices of binary feature operation context (discrete features to discrete features) with batch ability
        const vector<int>& batch_ddb_ctx() const;

        // Self load the index map of feature operation contexts
        bool self_load();

        // Match ccu operation contexts with input feature group names and find their indices
        vector<int> find_in_ccu(const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const;

        // Match ccb operation contexts with input feature group names and find their indices
        vector<int> find_in_ccb(const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const;

        // Match cdu operation contexts with input feature group names and find their indices
        vector<int> find_in_cdu(const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatGrp> &inp_feats) const;

        // Match ddb operation contexts with input feature group names and find their indices
        vector<int> find_in_ddb(const ::google::protobuf::Map<::std::string, ::algocomp::DiscFeatGrp> &inp_feats) const;
    };

}

#endif //ALGORITHM_COMPONENTS_FEATOPCTXPACK_H
