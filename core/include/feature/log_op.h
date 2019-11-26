//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_LOG_OP_H
#define ALGORITHM_COMPONENTS_LOG_OP_H

#include "feature_operator.h"
#include "featpb/cont_feat_data.pb.h"
#include <optional>
#include <spdlog/logger.h>

using namespace std;

namespace algocomp {

    // Declare referenced class
    class LogOpCtx;

    /**
     * Log operator
     *
     * Define logarithm transform from continuous feature to continuous feature.
     */
    class LogOp: public FeatureOperator {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    public:
        // Constructor
        LogOp();

        // Apply logarithm transform from continuous feature to continuous feature in runtime
        optional<ContFeatData>
        apply(const ContFeatData &inp_feat, const LogOpCtx &ctx, google::protobuf::Arena &arena);
    };

}


#endif //ALGORITHM_COMPONENTS_LOG_OP_H
