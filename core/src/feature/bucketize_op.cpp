//
// Created by qfeng on 2018-12-26.
//

#include "feature/bucketize_op.h"
#include <boost/optional.hpp>
#include <cmath>
#include "resource/resource_manager.h"
#include "log/algocomp_log.h"

namespace algocomp {
    BucketizeOp::BucketizeOp() {
        _logger = spdlog::get(ACLog::_logger_name);
    }

    optional<DiscFeatData> BucketizeOp::apply(const ContFeatData &inp_feat, const algocomp::BucketizeOpCtx &ctx,
                                              google::protobuf::Arena &arena) {
        try {
            if (isfinite(inp_feat.fval()) && inp_feat.fval() != 0) {
                if (ctx.boundaries().size() <= 0) {
                    _logger->warn("Empty boundaries for bucketize operator (gname={}, fname={}).", inp_feat.gname(),
                                  inp_feat.fname());
                    return std::nullopt;
                }
                // Apply bucketizing transform
                auto &boundaries = ctx.boundaries();
                auto first_bigger_it = std::upper_bound(boundaries.begin(), boundaries.end(), inp_feat.fval());
                auto bucket = first_bigger_it - boundaries.begin();
                auto &out_names = ctx.out_names(inp_feat.gname(), inp_feat.fname(), bucket);
                // Create output feature
                DiscFeatData *outp_feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                outp_feat->set_gname(out_names.first);
                outp_feat->set_fname(out_names.second);
                return *outp_feat;
            } else {
                _logger->warn("Feature value (gname={}, fname={}) is infinite or zero which will be discarded.",
                              inp_feat.gname(), inp_feat.fname());
                return std::nullopt;
            }
        } catch (const exception &ex) {
            _logger->error("Bucketized wrong feature (gname={}, fname={}): {}", inp_feat.gname(), inp_feat.fname(),
                           ex.what());
        }
        return std::nullopt;
    }
}