//
// Created by qfeng on 2019-01-07.
//

#ifndef ALGORITHM_COMPONENTS_CROSS_OP_CTX_H
#define ALGORITHM_COMPONENTS_CROSS_OP_CTX_H

#include "feature/feat_op_ctx.h"
#include "featpb/disc_feat_grp.pb.h"
#include <optional>
#include <string>
#include <log/algocomp_log.h>

using namespace std;

namespace algocomp {

    // Declare referenced class
    class CrossOp;

    /**
     * Cross operation context
     *
     * Define context of cross operation
     */
    class CrossOpCtx : public Disc2DiscBinaryOpCtx {
    protected:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Cross operator
        shared_ptr<CrossOp> _op;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        virtual bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

    public:
        // Declare referenced operator
        friend class CrossOp;

        // Constructor
        CrossOpCtx(shared_ptr<CrossOp> &op, const string op_name="cross") : Disc2DiscBinaryOpCtx(op_name), _op(op) {
            _logger = spdlog::get(ACLog::_logger_name);
        }

        // Apply cross transform from discrete features to discrete feature in runtime
        virtual optional<pair<string, DiscFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;

        // Apply batch cross transform from discrete features to discrete features in runtime
        virtual optional<map<string, DiscFeatGrp>>
        batch_apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;
    };

}


#endif //ALGORITHM_COMPONENTS_CROSS_OP_CTX_H
