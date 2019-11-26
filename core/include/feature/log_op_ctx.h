//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_LOG_OP_CTX_H
#define ALGORITHM_COMPONENTS_LOG_OP_CTX_H

#include "feature/feat_op_ctx.h"
#include <string>
#include <vector>
#include <optional>
#include "featpb/cont_feat_grp.pb.h"
#include <memory>
#include "log/algocomp_log.h"

using namespace std;

namespace algocomp {
    // Declare referenced class
    class LogOp;

    // Define type of logarithm operation
    enum class LogOpType {
        LOG10 = 0,
        LOG = 1,
        LOG2 = 2
    };

    /**
     * Log operation context
     *
     * Define context of logarithm operator (e.g., log type)
     */
    class LogOpCtx : public Cont2ContUnaryOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Logarithm type
        LogOpType _log_op_type = LogOpType::LOG10;

        // Logarithm operator
        shared_ptr<LogOp> _op;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Get the logarithm type
        const LogOpType log_type() const { return _log_op_type; }

    public:
        // Declare friend class
        friend class LogOp;

        // Constructor with logger
        LogOpCtx(shared_ptr<LogOp> &op) : Cont2ContUnaryOpCtx("log"), _op(op) {
            _logger = spdlog::get(ACLog::_logger_name);
        }

        // Apply logarithm transform from continuous feature to continuous feature in runtime
        optional<pair<string, ContFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;
    };

}


#endif //ALGORITHM_COMPONENTS_LOG_OP_CTX_H
