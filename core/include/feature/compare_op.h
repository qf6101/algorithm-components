//
// Created by qfeng on 2019-01-16.
//

#ifndef ALGORITHM_COMPONENTS_COSINE_OP_H
#define ALGORITHM_COMPONENTS_COSINE_OP_H

#include "feature_operator.h"
#include "featpb/cont_feat_data.pb.h"
#include <optional>
#include <spdlog/logger.h>
#include <featpb/cont_feat_grp.pb.h>

namespace algocomp {

    // Declared referenced class
    class CompOpCtx;

    /**
     * Compare operator
     *
     * Define compare transform from 2 continuous features or 2 sets of continuous features to continuous feature.
     */
    class CompareOp : public FeatureOperator {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    public:
        // Constructor
        CompareOp();

        // Apply compare transform from 2 continuous features to continuous feature in runtime
        optional<pair<string, ContFeatGrp>>
        apply(const ContFeatData &inp_feat1, const ContFeatData &inp_feat2, const CompOpCtx &ctx,
              google::protobuf::Arena &arena);

        // Apply compare transform from 2 sets of continuous feature to continuous feature in runtime
        optional<pair<string, ContFeatGrp>>
        apply(const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatData> &inp_feats1,
              const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatData> &inp_feats2,
              const CompOpCtx &ctx, google::protobuf::Arena &arena);

    };

}


#endif //ALGORITHM_COMPONENTS_COSINE_OP_H
