//
// Created by qfeng on 2019-01-04.
//

#ifndef ALGORITHM_COMPONENTS_BUCKETIZE_OP_CTX_H
#define ALGORITHM_COMPONENTS_BUCKETIZE_OP_CTX_H

#include <vector>
#include "feature/feat_op_ctx.h"
#include "log/algocomp_log.h"

using namespace std;

namespace algocomp {

    // Declare referenced class
    class BucketizeOp;

    /**
     * Bucketize Operator Context
     *
     * Define context of bucketize operator (e.g., boundaries).
     */
    class BucketizeOpCtx : public Cont2DiscUnaryOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Predefine ten-equal-width boundaries for min-max range (0 ~ 1)
        static vector<float> _ten_equal_width;

        // Predefine ten-equal-width boundaries for zscore range (p-value of 0.05 ~ 0.95)
        // We also provide gaussian to uniform transform followed by min-max range boundaries.
        // see "after zscore" pattern in real valued attribute operation context.
        static vector<float> _ten_equal_width_zscore;

        // Boundaries of buckets
        vector<float> _boundaries;

        // Bucketize operator
        shared_ptr<BucketizeOp> _op;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Get boundaries
        const vector<float> boundaries() const;

    public:
        // Declare friend class
        friend class BucketizeOp;

        // Constructor
        BucketizeOpCtx(shared_ptr<BucketizeOp> &op) : Cont2DiscUnaryOpCtx("bkt"), _op(op) {
            _logger = spdlog::get(ACLog::_logger_name);
        }

        // Apply bucketizing transform from continuous feature to discrete feature in runtime
        optional<pair<string, DiscFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;
    };

}


#endif //ALGORITHM_COMPONENTS_BUCKETIZE_OP_CTX_H
