//
// Created by qfeng on 2018-12-26.
//

#include "feature/feat_transer.h"
#include <boost/property_tree/xml_parser.hpp>
#include "feature/bucketize_op_ctx.h"
#include "feature/bucketize_op.h"
#include "feature/log_op.h"
#include "feature/log_op_ctx.h"
#include "feature/cross_op.h"
#include "feature/cross_op_ctx.h"
#include "feature/cross_all_op_ctx.h"
#include "feature/compare_op.h"
#include "feature/comp_op_ctx.h"
#include "resource/resource_manager.h"
#include <boost/filesystem.hpp>
#include <boost/thread/latch.hpp>
#include <google/protobuf/util/json_util.h>


using namespace boost::property_tree::xml_parser;

namespace algocomp {

    bool FeatTranser::create_ccu_opctx(const pt::ptree &ctx_item_,
                                       vector<shared_ptr<Cont2ContUnaryOpCtx>> &ccu_ctx,
                                       map<string, shared_ptr<algocomp::FeatureOperator>> &op_rsrc) {
        try {
            // Create ccu feature operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            auto op_type = ctx_item.get<string>("op");
            auto op_found = op_rsrc.find(op_type);
            if (op_found != op_rsrc.end()) {
                if (op_type == "log") {
                    shared_ptr<LogOp> op = std::dynamic_pointer_cast<LogOp>((*op_found).second);
                    auto ctx_ = make_shared<LogOpCtx>(op);
                    auto ctx = std::dynamic_pointer_cast<UnaryOpCtx>(ctx_);
                    parse_unary_patterns(ctx_item, ctx);
                    // Add the created and info-filled feature operation context
                    ccu_ctx.emplace_back(ctx_);
                    return true;
                }
            } else {
                _logger->error("Found not registered ccu feature operator {}", op_type);
            }
        } catch (const exception &ex) {
            _logger->error("Parsed ccu opctx feature (item={}) xml fail :{}", ctx_item_.data(), ex.what());
        }
        return false;
    }

    bool FeatTranser::create_ccb_opctx(const pt::ptree &ctx_item_, vector<shared_ptr<Cont2ContBinaryOpCtx>> &ccb_ctx,
                                       map<string, shared_ptr<FeatureOperator>> &op_rsrc) {
        try {
            // Create ccb feature operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            auto op_type = ctx_item.get<string>("op");
            auto op_found = op_rsrc.find(op_type);
            if (op_found != op_rsrc.end()) {
                if (op_type == "comp") {
                    shared_ptr<CompareOp> op = std::dynamic_pointer_cast<CompareOp>((*op_found).second);
                    auto ctx_ = make_shared<CompOpCtx>(op);
                    auto ctx = std::dynamic_pointer_cast<BinaryOpCtx>(ctx_);
                    parse_binary_patterns(ctx_item, ctx);
                    // Add the created and info-filled feature operation context
                    ccb_ctx.emplace_back(ctx_);
                    return true;
                }
            } else {
                _logger->error("Found not registered ccb feature operator {}", op_type);
            }
        } catch (const exception &ex) {
            _logger->error("Parsed ccb opctx feature (item={}) xml fail :{}", ctx_item_.data(), ex.what());
        }
        return false;
    }


    bool FeatTranser::create_cdu_opctx(const pt::ptree &ctx_item_,
                                       vector<shared_ptr<Cont2DiscUnaryOpCtx>> &cdu_ctx,
                                       map<string, shared_ptr<FeatureOperator>> &op_rsrc) {
        try {
            // Create cdu feature operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            auto op_type = ctx_item.get<string>("op");
            auto op_found = op_rsrc.find(op_type);
            if (op_found != op_rsrc.end()) {
                if (op_type == "bucketize") {
                    shared_ptr<BucketizeOp> op = std::dynamic_pointer_cast<BucketizeOp>((*op_found).second);
                    auto ctx_ = make_shared<BucketizeOpCtx>(op);
                    auto ctx = std::dynamic_pointer_cast<UnaryOpCtx>(ctx_);
                    parse_unary_patterns(ctx_item, ctx);
                    // Add the created and info-filled feature operation context
                    cdu_ctx.emplace_back(ctx_);
                    return true;
                }
            } else {
                _logger->error("Found not registered cdu feature operator {}", op_type);
            }
        } catch (const exception &ex) {
            _logger->error("Parsed cdu opctx feature (item={}) xml fail :{}", ctx_item_.data(), ex.what());
        }
        return false;
    }


    bool FeatTranser::create_ddb_opctx(const pt::ptree &ctx_item_,
                                       vector<shared_ptr<Disc2DiscBinaryOpCtx>> &ddb_ctx,
                                       map<string, shared_ptr<algocomp::FeatureOperator>> &op_rsrc) {
        try {
            // Create ddb feature operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            auto op_type = ctx_item.get<string>("op");
            auto op_found = op_rsrc.find(op_type);
            if (op_found != op_rsrc.end()) {
                if (op_type == "cross") {
                    shared_ptr<CrossOp> op = std::dynamic_pointer_cast<CrossOp>((*op_found).second);
                    auto ctx_ = make_shared<CrossOpCtx>(op);
                    auto ctx = std::dynamic_pointer_cast<BinaryOpCtx>(ctx_);
                    parse_binary_patterns(ctx_item, ctx);
                    // Add the created and info-filled feature operation context
                    ddb_ctx.emplace_back(ctx_);
                    return true;
                } else if (op_type == "crossall") {
                    shared_ptr<CrossOp> op = std::dynamic_pointer_cast<CrossOp>((*op_found).second);
                    auto ctx_ = make_shared<CrossAllOpCtx>(op);
                    auto ctx = std::dynamic_pointer_cast<BinaryOpCtx>(ctx_);
                    parse_binary_patterns(ctx_item, ctx);
                    // Add the created and info-filled feature operation context
                    ddb_ctx.emplace_back(ctx_);
                    return true;
                }
            } else {
                _logger->error("Found not registered ddb feature operator {}", op_type);
            }
        } catch (const exception &ex) {
            _logger->error("Parsed ddb opctx feature (item={}) xml fail :{}", ctx_item_.data(), ex.what());
        }

        return false;
    }

    bool FeatTranser::parse_unary_patterns(const pt::ptree &ctx_item,
                                           shared_ptr<algocomp::UnaryOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_gname = ctx_item.get<string>("in_gname");
        auto in_fname = ctx_item.get<string>("in_fname");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto op_scope = ctx_item.get<string>("scope");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_gname, in_fname, out_gname, out_fname, op_scope, proc_desc, param_desc)) return true;
        else return false;
    }

    bool FeatTranser::parse_binary_patterns(const pt::ptree &ctx_item,
                                            shared_ptr<algocomp::BinaryOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_gname1 = ctx_item.get<string>("in_gname1");
        auto in_fname1 = ctx_item.get<string>("in_fname1");
        auto in_gname2 = ctx_item.get<string>("in_gname2");
        auto in_fname2 = ctx_item.get<string>("in_fname2");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto op_scope = ctx_item.get<string>("scope");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_gname1, in_fname1, in_gname2, in_fname2, out_gname, out_fname, op_scope,
                                proc_desc, param_desc))
            return true;
        else return false;
    }

    FeatTranser::FeatTranser(const string &conf_fname, const string &conf_dname, shared_ptr<ctpl::thread_pool> thp_rsrc,
                             map<string, shared_ptr<algocomp::FeatureOperator>> &op_rsrc, const ThreadType thread_type)
            : _thp_rsrc(thp_rsrc), _thread_type(thread_type) {
        // Define Spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
        try {
            // Initialize feature transformer from xml configuration file
            pt::ptree tree;
            pt::read_xml(conf_dname + "/" + conf_fname, tree, trim_whitespace | no_comments);

            // uc_ccu
            {
                auto uc_ccu_item = tree.get_child_optional(
                        "aucimpr-feat-trans.user-and-ctx.unary-trans.cont2cont");
                if (uc_ccu_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_ccu_item.value()) {
                        if (ctx_item.first == "op-ctx") {
                            if (!create_ccu_opctx(ctx_item.second, _uc_opcs.ccu_ctx, op_rsrc)) {
                                _logger->error("Create uc_ccu feature operation context fail.");
                            }
                        }
                    }
                }
            }

            // uc_ccb
            {
                auto uc_ccb_item = tree.get_child_optional(
                        "aucimpr-feat-trans.user-and-ctx.binary-trans.cont2cont");
                if (uc_ccb_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_ccb_item.value()) {
                        if (ctx_item.first == "op-ctx") {
                            if (!create_ccb_opctx(ctx_item.second, _uc_opcs.ccb_ctx, op_rsrc)) {
                                _logger->error("Create uc_ccb feature operation context fail.");
                            }
                        }
                    }
                }
            }

            // uc_cdu
            {
                auto uc_cdu_item = tree.get_child_optional(
                        "aucimpr-feat-trans.user-and-ctx.unary-trans.cont2disc");
                if (uc_cdu_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_cdu_item.value()) {
                        if (!create_cdu_opctx(ctx_item.second, _uc_opcs.cdu_ctx, op_rsrc)) {
                            _logger->error("Create uc_cdu feature operation context fail.");
                        }
                    }
                }
            }

            // uc ddb
            {
                auto uc_ddb_item = tree.get_child_optional(
                        "aucimpr-feat-trans.user-and-ctx.binary-trans.disc2disc");
                if (uc_ddb_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_ddb_item.value()) {
                        if (!create_ddb_opctx(ctx_item.second, _uc_opcs.ddb_ctx, op_rsrc)) {
                            _logger->error("Create uc_ddb feature operation context fail.");
                        }
                    }
                }
            }

            // ad ccu
            {
                auto ad_ccu_item = tree.get_child_optional(
                        "aucimpr-feat-trans.ad.unary-trans.cont2cont");
                if (ad_ccu_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_ccu_item.value()) {
                        if (!create_ccu_opctx(ctx_item.second, _ad_opcs.ccu_ctx, op_rsrc)) {
                            _logger->error("Create ad_ccu feature operation context fail.");
                        }
                    }
                }
            }

            // ad_ccb
            {
                auto ad_ccb_item = tree.get_child_optional(
                        "aucimpr-feat-trans.ad.binary-trans.cont2cont");
                if (ad_ccb_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_ccb_item.value()) {
                        if (ctx_item.first == "op-ctx") {
                            if (!create_ccb_opctx(ctx_item.second, _ad_opcs.ccb_ctx, op_rsrc)) {
                                _logger->error("Create ad_ccb feature operation context fail.");
                            }
                        }
                    }
                }
            }

            // ad cdu
            {
                auto ad_cdu_item = tree.get_child_optional(
                        "aucimpr-feat-trans.ad.unary-trans.cont2disc");
                if (ad_cdu_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_cdu_item.value()) {
                        if (!create_cdu_opctx(ctx_item.second, _ad_opcs.cdu_ctx, op_rsrc)) {
                            _logger->error("Create ad_cdu feature operation context fail.");
                        }
                    }
                }
            }

            // ad ddb
            {
                auto ad_ddb_item = tree.get_child_optional(
                        "aucimpr-feat-trans.ad.binary-trans.disc2disc");
                if (ad_ddb_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_ddb_item.value()) {
                        if (!create_ddb_opctx(ctx_item.second, _ad_opcs.ddb_ctx, op_rsrc)) {
                            _logger->error("Create ad_ddb feature operation context fail.");
                        }
                    }
                }
            }

            // Self load the index map of feature operation contexts
            if (!_uc_opcs.self_load()) {
                _logger->error("Load uc feature opcs fail.");
            }
            if (!_ad_opcs.self_load()) {
                _logger->error("Load ad feature opcs fail.");
            }

            // log the size information
            _logger->info("Feature Transform: number of uc_ccu contexts {}, {}", _uc_opcs.ccu_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of uc_ccb contexts {}, {}", _uc_opcs.ccb_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of uc_cdu contexts {}, {}", _uc_opcs.cdu_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of uc_ddb contexts {}, {}", _uc_opcs.ddb_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of ad_ccu contexts {}, {}", _ad_opcs.ccu_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of ad_ccb contexts {}, {}", _ad_opcs.ccb_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of ad_cdu contexts {}, {}", _ad_opcs.cdu_ctx.size(), conf_fname);
            _logger->info("Feature Transform: number of ad_ddb contexts {}, {}", _ad_opcs.ddb_ctx.size(), conf_fname);
        } catch (const std::exception &ex) {
            boost::filesystem::path full_path(boost::filesystem::current_path());
            _logger->error("Initialize feature transformer fail: {}. Current path is: {}", ex.what(),
                           full_path.string());
        }
    }

    bool FeatTranser::apply(algocomp::AUCImprFeats &inp_feats, google::protobuf::Arena &arena) {
        bool success = true;
        try {
            if (_thread_type == ThreadType::ON_ADS) {
                // Apply the predefined transforms to the user-and-context part of input feature data with multi-thread
                if (!apply_uc_with_multi_thread(_uc_opcs, inp_feats, arena)) success = false;
                // Apply the predefined transforms to the ad part of input feature data with multi-thread
                if (!apply_ads_with_multi_thread(_ad_opcs, inp_feats, arena)) success = false;
            } else {
                // Apply the predefined transforms to the user-and-context part of input feature data
                if (!apply_uc(_uc_opcs, inp_feats, arena)) success = false;
                // Apply the predefined transforms to the ad part of input feature data
                if (!apply_ads(_ad_opcs, inp_feats, arena)) success = false;
            }

        } catch (const exception &ex) {
            _logger->error("Transform feature data fail {}.", ex.what());
            success = false;
        }
        return success;
    }

    bool FeatTranser::apply_uc(const algocomp::FeatOpCtxPack &opcs, algocomp::AUCImprFeats &inp_feats,
                               google::protobuf::Arena &arena) {
        bool success = true;

        try {
            // Find the candidate feature operation contexts
            auto ccu_idx = opcs.find_in_ccu(inp_feats.user_and_ctx().cont_feats());
            for (auto &idx: ccu_idx) {
                auto &ctx = opcs.ccu_ctx[idx];
                // For every candidate context, apply its transform
                auto out_cont_feats_ = ctx->apply(
                        vector<::google::protobuf::Map<string, ContFeatGrp>>{inp_feats.user_and_ctx().cont_feats()},
                        arena);
                if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                    auto &out_cont_feats = out_cont_feats_.value();
                    (*inp_feats.mutable_user_and_ctx()->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc ccu fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate feature operation contexts
            auto ccb_idx = opcs.find_in_ccb(inp_feats.user_and_ctx().cont_feats());
            for (auto &idx:ccb_idx) {
                auto &ctx = opcs.ccb_ctx[idx];
                // For every candidate context, apply its transform
                auto out_cont_feats_ = ctx->apply(
                        vector<::google::protobuf::Map<string, ContFeatGrp>>{inp_feats.user_and_ctx().cont_feats()},
                        arena);
                if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                    auto &out_cont_feats = out_cont_feats_.value();
                    (*inp_feats.mutable_user_and_ctx()->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc ccb fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate feature operation contexts
            auto cdu_idx = opcs.find_in_cdu(inp_feats.user_and_ctx().cont_feats());
            for (auto &idx:cdu_idx) {
                auto &ctx = opcs.cdu_ctx[idx];
                // For every candidate context, apply its transform
                auto out_disc_feats_ = ctx->apply(
                        vector<::google::protobuf::Map<string, ContFeatGrp>>{inp_feats.user_and_ctx().cont_feats()},
                        arena);
                if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                    auto &out_disc_feats = out_disc_feats_.value();
                    (*inp_feats.mutable_user_and_ctx()->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc cdu fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate feature operation contexts
            auto ddb_idx = opcs.find_in_ddb(inp_feats.user_and_ctx().disc_feats());

            for (auto &idx: ddb_idx) {
                auto ctx = opcs.ddb_ctx[idx];
                // For every candidate context, apply its transform
                auto out_disc_feats_ = ctx->apply(
                        vector<::google::protobuf::Map<string, DiscFeatGrp>>{inp_feats.user_and_ctx().disc_feats()},
                        arena);
                if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                    auto &out_disc_feats = out_disc_feats_.value();
                    (*inp_feats.mutable_user_and_ctx()->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on ddb fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate feature operation contexts
            auto batch_ddb_idx = opcs.batch_ddb_ctx();
            for (auto &idx:batch_ddb_idx) {
                auto ctx = opcs.ddb_ctx[idx];
                // For every candidate context, apply its transform
                auto out_disc_feats = ctx->batch_apply(
                        vector<::google::protobuf::Map<string, DiscFeatGrp>>{inp_feats.user_and_ctx().disc_feats()},
                        arena);
                if (out_disc_feats.has_value() && out_disc_feats.value().size() > 0) {
                    for (auto &feats : out_disc_feats.value()) {
                        if (feats.second.feats_size() > 0) {
                            (*inp_feats.mutable_user_and_ctx()->mutable_disc_feats())[feats.first] = feats.second;
                        }
                    }
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc batch ddb fail: {}", ex.what());
            success = false;
        }

        return success;
    }

    bool FeatTranser::apply_uc_with_multi_thread(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats,
                                                 google::protobuf::Arena &arena) {
        bool success = true;
        boost::latch count_down_latch(1);

        auto thread_func = [this, &opcs, &inp_feats, &arena, &success, &count_down_latch] (int id) {
            try {
                success = apply_uc(opcs, inp_feats, arena);
            } catch (const exception &ex) {
                _logger->warn("Transform uc with multi-thread (id={}) fail: {}", id, ex.what());
            }

            count_down_latch.count_down();
        };

        _thp_rsrc->push(thread_func);
        count_down_latch.wait();

        return success;
    }

    bool FeatTranser::apply_ads(const algocomp::FeatOpCtxPack &opcs, algocomp::AUCImprFeats &inp_feats,
                                google::protobuf::Arena &arena) {
        bool success = true;

        // Repeat the transforms on each ad
        auto *ads = inp_feats.mutable_ads();
        for (int i = 0; i < ads->size(); ++i) {
            auto *ad = ads->Mutable(i);

            try {
                // Find the candidate feature operation contexts
                auto ccu_idx = opcs.find_in_ccu(ad->cont_feats());
                for (auto &idx: ccu_idx) {
                    auto &ctx = opcs.ccu_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_cont_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{ad->cont_feats()}, arena);
                    if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                        auto &out_cont_feats = out_cont_feats_.value();
                        (*ad->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ccu fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto ccb_idx = opcs.find_in_ccb(ad->cont_feats());
                for (auto &idx: ccb_idx) {
                    auto &ctx = opcs.ccb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_cont_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{inp_feats.user_and_ctx().cont_feats(),
                                                                                 ad->cont_feats()}, arena);
                    if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                        auto &out_cont_feats = out_cont_feats_.value();
                        (*ad->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ccb fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto cdu_idx = opcs.find_in_cdu(ad->cont_feats());
                for (auto &idx: cdu_idx) {
                    auto &ctx = opcs.cdu_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{ad->cont_feats()}, arena);
                    if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                        auto &out_disc_feats = out_disc_feats_.value();
                        (*ad->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cdu fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto ddb_idx = opcs.find_in_ddb(ad->disc_feats());
                for (auto &idx: ddb_idx) {
                    auto &ctx = opcs.ddb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, DiscFeatGrp>>{
                                    inp_feats.user_and_ctx().disc_feats(),
                                    ad->disc_feats()}, arena);
                    if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                        auto &out_disc_feats = out_disc_feats_.value();
                        (*ad->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ddb fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto batch_ddb_idx = opcs.batch_ddb_ctx();
                for (auto &idx: batch_ddb_idx) {
                    auto &ctx = opcs.ddb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats = ctx->batch_apply(
                            vector<::google::protobuf::Map<string, DiscFeatGrp>>{
                                    inp_feats.user_and_ctx().disc_feats(),
                                    ad->disc_feats()}, arena);
                    if (out_disc_feats.has_value() && out_disc_feats.value().size() > 0) {
                        for (auto &feats : out_disc_feats.value()) {
                            if (feats.second.feats_size() > 0) {
                                (*ad->mutable_disc_feats())[feats.first] = feats.second;
                            }
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) batch ddb fail: {}", i, ex.what());
                success = false;
            }
        }

        return success;
    }

    bool FeatTranser::apply_ads_with_multi_thread(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats,
                                                  google::protobuf::Arena &arena) {
        bool success = true;

        auto *ads = inp_feats.mutable_ads();

        std:: mutex w_mutex;

        boost::latch count_down_latch(ads->size());

        auto thread_func = [this, &w_mutex, &ads, &opcs, &arena, &success, &inp_feats, &count_down_latch](int id,
                                                                                                int index) {
            auto *ad = ads->Mutable(index);
            try {
                // Find the candidate feature operation contexts
                auto ccu_idx = opcs.find_in_ccu(ad->cont_feats());
                for (auto &idx: ccu_idx) {
                    auto &ctx = opcs.ccu_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_cont_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{ad->cont_feats()}, arena);
                    if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                        auto &out_cont_feats = out_cont_feats_.value();
                        {
                            std::lock_guard<std::mutex> lock(w_mutex);
                            (*ad->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ccu fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto ccb_idx = opcs.find_in_ccb(ad->cont_feats());
                for (auto &idx: ccb_idx) {
                    auto &ctx = opcs.ccb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_cont_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{inp_feats.user_and_ctx().cont_feats(),
                                                                                 ad->cont_feats()}, arena);
                    if (out_cont_feats_.has_value() && out_cont_feats_.value().second.feats_size() > 0) {
                        auto &out_cont_feats = out_cont_feats_.value();
                        {
                            std::lock_guard<std::mutex> lock(w_mutex);
                            (*ad->mutable_cont_feats())[out_cont_feats.first] = out_cont_feats.second;
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ccb fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto cdu_idx = opcs.find_in_cdu(ad->cont_feats());
                for (auto &idx: cdu_idx) {
                    auto &ctx = opcs.cdu_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, ContFeatGrp>>{ad->cont_feats()}, arena);
                    if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                        auto &out_disc_feats = out_disc_feats_.value();
                        {
                            std::lock_guard<std::mutex> lock(w_mutex);
                            (*ad->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cdu fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto ddb_idx = opcs.find_in_ddb(ad->disc_feats());
                for (auto &idx: ddb_idx) {
                    auto &ctx = opcs.ddb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats_ = ctx->apply(
                            vector<::google::protobuf::Map<string, DiscFeatGrp>>{
                                    inp_feats.user_and_ctx().disc_feats(),
                                    ad->disc_feats()}, arena);
                    if (out_disc_feats_.has_value() && out_disc_feats_.value().second.feats_size() > 0) {
                        auto &out_disc_feats = out_disc_feats_.value();
                        {
                            std::lock_guard<std::mutex> lock(w_mutex);
                            (*ad->mutable_disc_feats())[out_disc_feats.first] = out_disc_feats.second;
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) ddb fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate feature operation contexts
                auto batch_ddb_idx = opcs.batch_ddb_ctx();
                for (auto &idx: batch_ddb_idx) {
                    auto &ctx = opcs.ddb_ctx[idx];
                    // For every candidate context, apply its transform
                    auto out_disc_feats = ctx->batch_apply(
                            vector<::google::protobuf::Map<string, DiscFeatGrp>>{
                                    inp_feats.user_and_ctx().disc_feats(),
                                    ad->disc_feats()}, arena);
                    if (out_disc_feats.has_value() && out_disc_feats.value().size() > 0) {
                        for (auto &feats : out_disc_feats.value()) {
                            if (feats.second.feats_size() > 0) {
                                {
                                    std::lock_guard<std::mutex> lock(w_mutex);
                                    (*ad->mutable_disc_feats())[feats.first] = feats.second;
                                }
                            }
                        }
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) batch ddb fail: {}", index, ex.what());
                success = false;
            }

            count_down_latch.count_down();
        };

        for (int i = 0; i < ads->size(); ++i) {
            _thp_rsrc->push(thread_func, i);
        }

        count_down_latch.wait();

        return success;
    }
}
