//
// Created by qfeng on 2019-01-08.
//

#include "feature/log_op.h"
#include "feature/log_op_ctx.h"
#include <cmath>
#include <google/protobuf/arena.h>
#include "log/algocomp_log.h"

namespace algocomp {

    LogOp::LogOp() {
        // Define spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
    }

    optional<ContFeatData>
    LogOp::apply(const ContFeatData &inp_feat, const LogOpCtx &ctx, google::protobuf::Arena &arena) {
        try {
            if (isfinite(inp_feat.fval()) && inp_feat.fval() > 0) {
                float log_val = 0;

                // Apply log transform
                if (ctx.log_type() == LogOpType::LOG10) {
                    log_val = log10(inp_feat.fval());
                } else if (ctx.log_type() == LogOpType::LOG) {
                    log_val = log(inp_feat.fval());
                } else if (ctx.log_type() == LogOpType::LOG2) {
                    log_val = log2(inp_feat.fval());
                }

                // Create output feature
                auto &out_names = ctx.out_names(inp_feat.gname(), inp_feat.fname(), log_val);
                ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                outp_feat->set_gname(out_names.first);
                outp_feat->set_fname(out_names.second);
                outp_feat->set_fval(log_val);
                return *outp_feat;
            } else {
                _logger->warn("Feature value (gname={}, fname={}) is infinite or not greater than zero which will be discarded.",
                              inp_feat.gname(), inp_feat.fname());
                return std::nullopt;
            }
        } catch (const exception &ex) {
            _logger->error("Log wised wrong feature (gname={}, fname={}): {}", inp_feat.gname(), inp_feat.fname(),
                           ex.what());
        }
        return std::nullopt;
    }
}
