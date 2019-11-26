//
// Created by qfeng on 2018-12-26.
//

#ifndef ALGOCOMP_BUCKETIZEOP_H
#define ALGOCOMP_BUCKETIZEOP_H

#include <vector>
#include "featpb/disc_feat_data.pb.h"
#include "featpb/cont_feat_data.pb.h"
#include "feature/bucketize_op_ctx.h"
#include "feature/feature_operator.h"
#include <optional>
#include "spdlog/logger.h"

namespace algocomp {

    // Declare referenced class
    class BucketizeOpCtx;

    /**
     * Bucketize Operator
     *
     * Define bucketizing transform from continuous feature to discrete feature.
     */
    class BucketizeOp : public FeatureOperator {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    public:
        // Constructor
        BucketizeOp();

        // Apply bucketizing transform from contiuous feature to discrete feature in runtime
        optional<DiscFeatData>
        apply(const ContFeatData &inp_feat, const BucketizeOpCtx &ctx, google::protobuf::Arena &arena);
    };

}

#endif //ALGOCOMP_BUCKETIZEOP_H
