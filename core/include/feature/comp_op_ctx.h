//
// Created by qfeng on 2019-01-16.
//

#ifndef ALGORITHM_COMPONENTS_COSINE_OP_CTX_H
#define ALGORITHM_COMPONENTS_COSINE_OP_CTX_H

#include "feat_op_ctx.h"
#include <vector>
#include <optional>
#include "featpb/cont_feat_grp.pb.h"
#include <memory>
#include "log/algocomp_log.h"

namespace algocomp {

    // Declare referenced class
    class CompareOp;

    /**
     * Compare operation context
     *
     * Define context of compare operation (e.g., compare type)
     */
    class CompOpCtx : public Cont2ContBinaryOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Compare operator
        const shared_ptr<CompareOp> _op;

        // Compare type
        bool _avg_dot = false;
        bool _avg_min = false;
        bool _avg_max = false;
        bool _emb_cos = false;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

    public:
        // Declare friend class
        friend class CompareOp;

        // Constructor
        CompOpCtx(shared_ptr<CompareOp> &op) : Cont2ContBinaryOpCtx("comp"), _op(op) {
            _logger = spdlog::get(ACLog::_logger_name);
        };

        // Apply compare transform from continuous features to continuous feature in runtime
        optional<pair<string, ContFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;
    };

}

#endif //ALGORITHM_COMPONENTS_COSINE_OP_CTX_H
