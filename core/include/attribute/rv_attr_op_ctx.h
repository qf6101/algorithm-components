//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_RV_ATTR_OP_CTX_H
#define ALGORITHM_COMPONENTS_RV_ATTR_OP_CTX_H

#include "attr_op_ctx.h"
#include "featpb/feature_bunch.pb.h"
#include <vector>
#include <limits>
#include <spdlog/logger.h>

using namespace std;

namespace algocomp {

    /**
     * Real valued attribute operation context
     *
     * Define transformation from real valued attribute to continuous feature.
     * The order is: (1) over range (too large or small) rule; (2) log computation; (3) value scale.
     */
    class RVAttrOpCtx : public AttrOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Define pattern of action for over large or small value
        enum class _OVER_RANGE_RULE {
            SKIP_DATA = 1,
            REPLACE_WITH_MIN_OR_MAX = 2
        };

        // Action type for over large or small value
        _OVER_RANGE_RULE _over_range_rule = _OVER_RANGE_RULE::SKIP_DATA;

        // Initial minimum and maximum values (as compared to measure too large or small)
        float _range_min = std::numeric_limits<float>::min();
        float _range_max = std::numeric_limits<float>::max();

        // Define pattern of logarithm computation type
        enum class _LogType {
            NO = 0,
            LOG10 = 1,
            LOG = 2,
            LOG2 = 3
        };

        // Logarithm computation type
        _LogType _log_type = _LogType::NO;

        // Minimum negative value (must less than zero) for adjusting the input negative attribute value before
        // logarithm computation: x = x - _min_neg_value, followed by x = log(x)
        float _min_neg_val = 0;

        // Define pattern of value scale action
        enum class _SCALE_TYPE {
            NONE,
            MIN_MAX = 1,
            Z_SCORE = 2
        };

        // Value scale action
        _SCALE_TYPE _scale_type = _SCALE_TYPE::NONE;

        // Parameters of value scale (have different meanings for different scale type)
        float _scale_param1 = 0;
        float _scale_param2 = 0;

        // Define pattern of action after zscore scaling
        enum class _AFTER_ZSCORE {
            GAUSSIAN_TO_UNIFORM = 1, // erfc operation for transfoming gaussian distribution to uniform distribution
            STAY_GAUSSIAN = 2
        };

        // Action after zscore scaling
        _AFTER_ZSCORE _after_zscore = _AFTER_ZSCORE::GAUSSIAN_TO_UNIFORM;

    public:
        // Constructor
        RVAttrOpCtx();

        // Parse processes together with their parameters for computing feature value from attribute value
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Apply transform from real valued attribute to continuous feature in runtime
        bool apply(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats, google::protobuf::Arena &arena) override;
    };

}


#endif //ALGORITHM_COMPONENTS_RV_ATTR_OP_CTX_H
