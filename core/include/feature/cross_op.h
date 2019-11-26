//
// Created by qfeng on 2019-01-07.
//

#ifndef ALGORITHM_COMPONENTS_CROSS_OP_H
#define ALGORITHM_COMPONENTS_CROSS_OP_H

#include "feature_operator.h"
#include <optional>
#include "featpb/disc_feat_data.pb.h"
#include "spdlog/logger.h"

using namespace std;


namespace algocomp {

    // Declare referenced class
    class CrossOpCtx;

    /**
     * Cross Operator
     *
     * Define cross transform from discrete features to discrete feature
     */
    class CrossOp : public FeatureOperator {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    public:
        // Constructor
        CrossOp();

        // Apply cross transform from discrete features to discrete feature in runtime
        optional<DiscFeatData>
        apply(const DiscFeatData &inp_feat1, const DiscFeatData &inp_feat2, const CrossOpCtx &ctx,
              google::protobuf::Arena &arena, const string how_cross="", const bool ignore_double_cross=false);
    };

}


#endif //ALGORITHM_COMPONENTS_CROSS_OP_H
