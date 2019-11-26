//
// Created by qfeng on 2019-01-16.
//

#include "feature/compare_op.h"
#include <cmath>
#include <google/protobuf/arena.h>
#include "log/algocomp_log.h"
#include "feature/comp_op_ctx.h"

namespace algocomp {

    CompareOp::CompareOp() {
        // Define spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
    }


    optional<pair<string, ContFeatGrp>> CompareOp::apply(const algocomp::ContFeatData &inp_feat1,
                                                         const algocomp::ContFeatData &inp_feat2,
                                                         const algocomp::CompOpCtx &ctx,
                                                         google::protobuf::Arena &arena) {
        try {
            if (isfinite(inp_feat1.fval()) && isfinite(inp_feat2.fval()) &&
                inp_feat1.fval() != 0 && inp_feat2.fval() != 0) {

                ContFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<ContFeatGrp>(&arena);
                string out_gname = "";

                if (ctx._avg_dot) {
                    // Apply compare transform
                    auto res = inp_feat1.fval() * inp_feat2.fval();
                    auto &out_names = ctx.out_names(inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(),
                                                    inp_feat2.fname(), "avg_dot");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (ctx._avg_min) {
                    // Apply compare transform
                    auto res = min(inp_feat1.fval(), inp_feat2.fval());
                    auto &out_names = ctx.out_names(inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(),
                                                    inp_feat2.fname(), "avg_min");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (ctx._avg_max) {
                    // Apply compare transform
                    auto res = max(inp_feat1.fval(), inp_feat2.fval());
                    auto &out_names = ctx.out_names(inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(),
                                                    inp_feat2.fname(), "avg_max");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (out_gname.length() > 0 && outp_feats->feats_size() > 0) {
                    return make_pair(out_gname, *outp_feats);
                } else {
                    _logger->error("Empty out gname composed (gname1={}, fname1={}, gname2={}, fname2={})",
                                   inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(), inp_feat2.fname());
                }
            } else {
                _logger->warn(
                        "At least one of input feature values is infinite or zero (gname1={}, fname1={}, gname2={}, fname2={})",
                        inp_feat1.gname(), inp_feat1.fname(), inp_feat2.gname(), inp_feat2.fname());
            }
        } catch (const exception &ex) {
            _logger->error("Compare wrong features (gname1={}, fname1={}, gname2=P{, fname2={})", inp_feat1.gname(),
                           inp_feat1.fname(), inp_feat2.gname(), inp_feat2.fname());

        }

        return std::nullopt;
    }

    optional<pair<string, ContFeatGrp>> CompareOp::apply(
            const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatData> &inp_feats1,
            const ::google::protobuf::Map<::std::string, ::algocomp::ContFeatData> &inp_feats2,
            const algocomp::CompOpCtx &ctx, google::protobuf::Arena &arena) {
        string gname1 = "";
        string gname2 = "";
        string out_gname = "";

        try {
            // Compute the average values of 2 sets of features
            float sum1 = 0;
            int count1 = 0;
            float avg1 = 0;

            for (auto &inp_feat1: inp_feats1) {
                if (isfinite(inp_feat1.second.fval())) {
                    sum1 += inp_feat1.second.fval();
                    count1 += 1;
                    gname1 = inp_feat1.first;
                }
            }
            if (count1 > 0) avg1 = sum1 / count1;

            float sum2 = 0;
            int count2 = 0;
            float avg2 = 0;

            for (auto &inp_feat2: inp_feats2) {
                if (isfinite(inp_feat2.second.fval())) {
                    sum2 += inp_feat2.second.fval();
                    count2 += 1;
                    gname2 = inp_feat2.first;
                }
            }
            if (count2 > 0) avg2 = sum2 / count2;

            if (avg1 != 0 && avg2 != 0 && gname1.length() > 0 && gname2.length() > 0) {

                ContFeatGrp *outp_feats = google::protobuf::Arena::CreateMessage<ContFeatGrp>(&arena);

                if (ctx._avg_dot) {
                    // Apply compare transform
                    auto res = avg1 * avg2;
                    auto &out_names = ctx.out_names(gname1, gname1, gname2, gname2, "avg_dot");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (ctx._avg_min) {
                    // Apply compare transform
                    auto res = min(avg1, avg2);
                    auto &out_names = ctx.out_names(gname1, gname1, gname2, gname2, "avg_min");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (ctx._avg_max) {
                    // Apply compare transform
                    auto res = max(avg1, avg2);
                    auto &out_names = ctx.out_names(gname1, gname1, gname2, gname2, "avg_max");
                    ContFeatData *outp_feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    outp_feat->set_gname(out_names.first);
                    outp_feat->set_fname(out_names.second);
                    outp_feat->set_fval(res);
                    (*outp_feats->mutable_feats())[out_names.second] = *outp_feat;
                    out_gname = out_names.first;
                }

                if (out_gname.length() > 0 && outp_feats->feats_size() > 0) {
                    return make_pair(out_gname, *outp_feats);
                } else {
                    _logger->error("Empty out gname composed (gname1={}, gname2={})", gname1, gname2);
                }
            } else {
                _logger->warn(
                        "At least one of average feature values is infinite or zero (gname1={}, gname2={})",
                        gname1, gname2);
            }

        } catch (const exception &ex) {
            _logger->error("Compare wrong features (batch) (gname1={}, gname2={})", gname1, gname2);
        }

        return std::nullopt;
    }

}