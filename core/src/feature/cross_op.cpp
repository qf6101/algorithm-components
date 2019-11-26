//
// Created by qfeng on 2019-01-07.
//

#include "feature/cross_op.h"
#include "feature/cross_op_ctx.h"
#include "resource/resource_manager.h"
#include "log/algocomp_log.h"
#include "absl/strings/match.h"

namespace algocomp {

    CrossOp::CrossOp() {
        _logger = spdlog::get(ACLog::_logger_name);
    }

    optional<DiscFeatData> CrossOp::apply(const DiscFeatData &inp_feat1, const DiscFeatData &inp_feat2,
                                          const algocomp::CrossOpCtx &ctx, google::protobuf::Arena &arena,
                                          const string how_cross, const bool ignore_double_cross) {
        try {
            bool double_cross = false;

            // Check if one of the input features is already the crossed features
            if (absl::StartsWith(inp_feat1.gname(), "!cross") ||
                absl::StartsWith(inp_feat2.gname(), "!cross")) {
                if (ignore_double_cross) return std::nullopt;
                else double_cross = true;
            }

            // Apply cross transform
            pair<string, string> out_names;
            if (inp_feat1.gname().compare(inp_feat2.gname()) < 0) {
                out_names = ctx.out_names(inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(),
                                          inp_feat2.fname(), how_cross, double_cross);
            } else {
                out_names = ctx.out_names(inp_feat2.gname(), inp_feat2.fname(), inp_feat1.gname(),
                                          inp_feat1.fname(), how_cross, double_cross);
            }

            // Create output feature
            DiscFeatData *outp_feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
            outp_feat->set_gname(out_names.first);
            outp_feat->set_fname(out_names.second);
            return *outp_feat;
        } catch (const exception &ex) {
            _logger->error("Crossed wrong features (gname1={}, fname1={}, gname2={}, fname2={})", inp_feat1.gname(),
                           inp_feat1.fname(), inp_feat2.gname(), inp_feat2.fname());
        }
        return std::nullopt;
    }

}
