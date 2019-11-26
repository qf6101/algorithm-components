//
// Created by qfeng on 2018-12-26.
//

#include "attribute/attr_transer.h"
#include "resource/resource_manager.h"
#include <boost/thread/latch.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <google/protobuf/util/json_util.h>


using namespace boost::property_tree::xml_parser;

namespace algocomp {

    bool AttrTranser::create_setcate_opctx(const pt::ptree &ctx_item_,
                                           vector<shared_ptr<algocomp::SetCateAttrOpCtx>> &setcate_ctx) {
        try {
            // Create set-categorical attribute operation context and parse pattern information fro xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            shared_ptr<SetCateAttrOpCtx> ctx = make_shared<SetCateAttrOpCtx>();
            if (parse_setcate_patterns(ctx_item, ctx)) {
                // Add the created and info-filled set-categorical attribute operation context
                setcate_ctx.emplace_back(std::move(ctx));
                return true;
            } else {
                _logger->error("Parse set-categorical attribute patterns fail");
                return false;
            }
        } catch (const exception &ex) {
            _logger->error("Parsed set-categorical attribute xml (item={}) fail: {}.", ctx_item_.data(), ex.what());

        }

        return false;

    }

    bool AttrTranser::create_cate_opctx(const pt::ptree &ctx_item_,
                                        vector<shared_ptr<algocomp::CateAttrOpCtx>> &cate_ctx) {
        try {
            // Create categorical attribute operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            shared_ptr<CateAttrOpCtx> ctx = make_shared<CateAttrOpCtx>();
            if (parse_cate_patterns(ctx_item, ctx)) {
                // Add the created and info-filled categorical attribute operation context
                cate_ctx.emplace_back(std::move(ctx));
                return true;
            } else {
                _logger->error("Parse categorical attribute patterns fail");
                return false;
            }
        } catch (const exception &ex) {
            _logger->error("Parsed categorical attribute xml (item={}) fail: {}.", ctx_item_.data(), ex.what());

        }

        return false;
    }

    bool AttrTranser::create_rv_opctx(const pt::ptree &ctx_item_,
                                      vector<shared_ptr<algocomp::RVAttrOpCtx>> &rv_ctx) {
        try {
            // Create real valued attribute operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            shared_ptr<RVAttrOpCtx> ctx = make_shared<RVAttrOpCtx>();
            if (parse_rv_patterns(ctx_item, ctx)) {
                // Add the created and info-filled real valued attribute operation context
                rv_ctx.emplace_back(std::move(ctx));
                return true;
            } else {
                _logger->error("Parse real valued attribute patterns fail.");
                return false;
            }
        } catch (const exception &ex) {
            _logger->error("Parsed real valued attribute xml (item={}) fail: {}.", ctx_item_.data(), ex.what());

        }

        return false;
    }

    bool AttrTranser::create_time_opctx(const pt::ptree &ctx_item_,
                                        vector<shared_ptr<algocomp::TimeAttrOpCtx>> &time_ctx) {
        try {
            // Create time attribute operation context and parse pattern information from xml mode
            auto ctx_item = ctx_item_.get_child("<xmlattr>");
            shared_ptr<TimeAttrOpCtx> ctx = make_shared<TimeAttrOpCtx>();
            if (parse_time_patterns(ctx_item, ctx)) {
                // Add the created and info-filled time attribute operation context
                time_ctx.emplace_back(std::move(ctx));
                return true;
            } else {
                _logger->error("Parse time attribute patterns fail.");
                return false;
            }
        } catch (const exception &ex) {
            _logger->error("Parsed time attribute xml (item={}) fail: {}.", ctx_item_.data(), ex.what());

        }

        return false;
    }

    bool AttrTranser::parse_cate_patterns(const pt::ptree &ctx_item,
                                          shared_ptr<algocomp::CateAttrOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_aname = ctx_item.get<string>("in_aname");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_aname, out_gname, out_fname, proc_desc, param_desc)) return true;
        else return false;
    }

    bool AttrTranser::parse_rv_patterns(const pt::ptree &ctx_item,
                                        shared_ptr<algocomp::RVAttrOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_aname = ctx_item.get<string>("in_aname");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_aname, out_gname, out_fname, proc_desc, param_desc)) return true;
        else return false;
    }

    bool AttrTranser::parse_time_patterns(const pt::ptree &ctx_item, shared_ptr<algocomp::TimeAttrOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_aname = ctx_item.get<string>("in_aname");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_aname, out_gname, out_fname, proc_desc, param_desc)) return true;
        else return false;
    }

    bool AttrTranser::parse_setcate_patterns(const pt::ptree &ctx_item, shared_ptr<algocomp::SetCateAttrOpCtx> &ctx) {
        // Parse string information from xml node
        auto in_aname = ctx_item.get<string>("in_aname");
        auto out_gname = ctx_item.get<string>("out_gname");
        auto out_fname = ctx_item.get<string>("out_fname");
        auto proc_desc = ctx_item.get<string>("procs", "");
        auto param_desc = ctx_item.get<string>("params", "");
        // Parse content in the string information
        if (ctx->parse_patterns(in_aname, out_gname, out_fname, proc_desc, param_desc)) {
            return true;
        } else {
            return false;
        }
    }

    AttrTranser::AttrTranser(const std::string &conf_fname, const std::string &conf_dname,
                             shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type) : _thp_rsrc(
            thp_rsrc), _thread_type(thread_type) {
        // Define Spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
        try {
            // Initialize attribute transformer from xml configuration file
            pt::ptree tree;
            pt::read_xml(conf_dname + "/" + conf_fname, tree, trim_whitespace | no_comments);

            // uc set-categorical
            {
                auto uc_item = tree.get_child_optional("aucimpr-attributes.user-and-ctx.setcate-attrs");

                if (uc_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_item.value()) {
                        if (!create_setcate_opctx(ctx_item.second, _uc_opcs.setcate_ctx)) {
                            _logger->error("Create uc set-categorical operation context fail.");
                        }
                    }
                }
            }

            // uc categorical
            {
                auto uc_item = tree.get_child_optional("aucimpr-attributes.user-and-ctx.categorical-attrs");
                if (uc_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_item.value()) {
                        if (!create_cate_opctx(ctx_item.second, _uc_opcs.cate_ctx)) {
                            _logger->error("Create uc categorical operation context fail.");
                        }
                    }
                }
            }

            // uc realvalued
            {
                auto uc_item = tree.get_child_optional("aucimpr-attributes.user-and-ctx.realvalued-attrs");
                if (uc_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_item.value()) {
                        if (!create_rv_opctx(ctx_item.second, _uc_opcs.rv_ctx)) {
                            _logger->error("Create uc real valued operation context fail.");
                        }
                    }
                }
            }

            // uc time
            {
                auto uc_item = tree.get_child_optional("aucimpr-attributes.user-and-ctx.time-attrs");
                if (uc_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: uc_item.value()) {
                        if (!create_time_opctx(ctx_item.second, _uc_opcs.time_ctx)) {
                            _logger->error("Create uc time operation context fail.");
                        }
                    }
                }
            }

            // ad set-categorical
            {
                auto ad_item = tree.get_child_optional("aucimpr-attributes.ads.setcate-attrs");
                if (ad_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_item.value()) {
                        if (!create_setcate_opctx(ctx_item.second, _ad_opcs.setcate_ctx)) {
                            _logger->error("Create ad set-categorical operation context fail.");
                        }
                    }
                }
            }

            // ad categorical
            {
                auto ad_item = tree.get_child_optional("aucimpr-attributes.ads.categorical-attrs");
                if (ad_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_item.value()) {
                        if (!create_cate_opctx(ctx_item.second, _ad_opcs.cate_ctx)) {
                            _logger->error("Create ad categorical operation context fail.");
                        }
                    }
                }
            }

            // ad realvalued
            {
                auto ad_item = tree.get_child_optional("aucimpr-attributes.ads.realvalued-attrs");
                if (ad_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_item.value()) {
                        if (!create_rv_opctx(ctx_item.second, _ad_opcs.rv_ctx)) {
                            _logger->error("Create ad real valued operation context fail.");
                        }
                    }
                }
            }

            // ad time
            {
                auto ad_item = tree.get_child_optional("aucimpr-attributes.ads.time-attrs");
                if (ad_item.is_initialized()) {
                    for (const pt::ptree::value_type &ctx_item: ad_item.value()) {
                        if (!create_time_opctx(ctx_item.second, _ad_opcs.time_ctx)) {
                            _logger->error("Create ad time operation context fail.");
                        }
                    }
                }
            }

            // Self load the index map of attribute operation contexts
            if (!_uc_opcs.self_load()) {
                _logger->error("Load uc attribute opcs fail.");
            }
            if (!_ad_opcs.self_load()) {
                _logger->error("Load ad attribute opcs fail.");
            }

            // log the size information
            _logger->info("Attribute Transform: number of uc_cate contexts {}, {}", _uc_opcs.cate_ctx.size(),
                          conf_fname);
            _logger->info("Attribute Transform: number of uc_setcate contexts {}, {}", _uc_opcs.setcate_ctx.size(),
                          conf_fname);
            _logger->info("Attribute Transform: number of uc_rv contexts {}, {}", _uc_opcs.rv_ctx.size(), conf_fname);
            _logger->info("Attribute Transform: number of uc_time contexts {}, {}", _uc_opcs.time_ctx.size(),
                          conf_fname);
            _logger->info("Attribute Transform: number of ad_cate contexts {}, {}", _ad_opcs.cate_ctx.size(),
                          conf_fname);
            _logger->info("Attribute Transform: number of ad_setcate contexts {}, {}", _ad_opcs.setcate_ctx.size(),
                          conf_fname);
            _logger->info("Attribute Transform: number of ad_rv contexts {}, {}", _ad_opcs.rv_ctx.size(), conf_fname);
            _logger->info("Attribute Transform: number of ad_time contexts {}, {}", _ad_opcs.time_ctx.size(),
                          conf_fname);

        } catch (const exception &ex) {
            _logger->error("Initialize attribute transformer fail {}.", ex.what());
        }
    }

    optional<AUCImprFeats> AttrTranser::apply(const algocomp::AUCImprAttrs &inp_attrs, google::protobuf::Arena &arena) {
        try {
            // Create the output feature data and fill it thru the following transforms
            AUCImprFeats *outp_feats = google::protobuf::Arena::CreateMessage<AUCImprFeats>(&arena);

            if (_thread_type == ThreadType::ON_ADS) {
                // Apply the predefined transforms to the user-and-context part of input attribute data with multi-thread
                if (!apply_uc_with_multi_thread(inp_attrs.user_and_ctx(), *outp_feats->mutable_user_and_ctx(), arena)) {
                }
                // Apply the predefined transforms to the ad part of input attribute data with multi-thread
                if (!apply_ads_with_multi_thread(inp_attrs, *outp_feats, arena)) {
                }
            } else {
                // Apply the predefined transforms to the user-and-context part of input attribute data
                if (!apply_uc(inp_attrs.user_and_ctx(), *outp_feats->mutable_user_and_ctx(), arena)) {
                }
                // Apply the predefined transforms to the ad part of input attribute data
                if (!apply_ads(inp_attrs, *outp_feats, arena)) {
                }
            }

            // Copy the labels from intput attribute data to output feature data
            if (!apply_labels(inp_attrs.labels(), *outp_feats->mutable_labels())) {
            }
            return *outp_feats;
        } catch (const exception &ex) {
            _logger->error("Transform attribute data fail {}.", ex.what());
        }
        return std::nullopt;
    }


    bool AttrTranser::apply_uc(const algocomp::AttributeBunch &inp_attrs,
                               algocomp::FeatureBunch &outp_feats, google::protobuf::Arena &arena) {
        bool success = true;
        try {
            // Find the candidate attribute operation contexts
            auto cate_idx = _uc_opcs.find_in_cate(inp_attrs.attrs());
            for (auto &idx: cate_idx) {
                auto &ctx = _uc_opcs.cate_ctx[idx];
                // For every candidate context, apply its transform
                if (!ctx->apply(inp_attrs, outp_feats, arena)) {
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc cate fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate attribute operation contexts
            auto setcate_idx = _uc_opcs.find_in_setcate(inp_attrs.attrs());
            for (auto &idx: setcate_idx) {
                auto &ctx = _uc_opcs.setcate_ctx[idx];
                // For every candidate context, apply its transform
                if (!ctx->apply(inp_attrs, outp_feats, arena)) {
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc setcate fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate attribute operation contexts
            auto rv_idx = _uc_opcs.find_in_rv(inp_attrs.attrs());
            for (auto &idx: rv_idx) {
                auto &ctx = _uc_opcs.rv_ctx[idx];
                // For every candidate context, apply its transform
                if (!ctx->apply(inp_attrs, outp_feats, arena)) {
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc rv fail: {}", ex.what());
            success = false;
        }

        try {
            // Find the candidate attribute operation contexts
            auto time_idx = _uc_opcs.find_in_time(inp_attrs.attrs());
            for (auto &idx: time_idx) {
                auto &ctx = _uc_opcs.time_ctx[idx];
                // For every candidate context, apply its transform
                if (!ctx->apply(inp_attrs, outp_feats, arena)) {
                }
            }
        } catch (const exception &ex) {
            _logger->warn("Transform on uc time fail: {}", ex.what());
            success = false;
        }

        return success;
    }

    bool AttrTranser::apply_uc_with_multi_thread(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats,
                                                 google::protobuf::Arena &arena) {
        bool success = true;
        boost::latch count_down_latch(1);

        auto thread_func = [this, &inp_attrs, &outp_feats, &arena, &success, &count_down_latch] (int id) {
            try {
                success = apply_uc(inp_attrs, outp_feats, arena);
            } catch (const exception &ex) {
                _logger->warn("Transform uc with multi-thread (id={}) fail: {}", id, ex.what());
            }

            count_down_latch.count_down();
        };

        _thp_rsrc->push(thread_func);
        count_down_latch.wait();

        return success;
    }

    bool AttrTranser::apply_ads(const algocomp::AUCImprAttrs &inp_attrs,
                                algocomp::AUCImprFeats &outp_feats, google::protobuf::Arena &arena) {
        bool success = true;

        // Repeat the transforms on each ad
        for (int i = 0; i < inp_attrs.ads_size(); ++i) {
            auto &ad = inp_attrs.ads(i);
            auto *ad_feats = google::protobuf::Arena::CreateMessage<FeatureBunch>(&arena);

            try {
                // Find the candidate attribute operation contexts
                auto cate_idx = _ad_opcs.find_in_cate(ad.attrs());
                for (auto &idx: cate_idx) {
                    auto &ctx = _ad_opcs.cate_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }

                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cate fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto setcate_idx = _ad_opcs.find_in_setcate(ad.attrs());
                for (auto &idx: setcate_idx) {
                    auto &ctx = _ad_opcs.setcate_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cate fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto rv_idx = _ad_opcs.find_in_rv(ad.attrs());
                for (auto &idx: rv_idx) {
                    auto &ctx = _ad_opcs.rv_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad(id={}) rv fail: {}", i, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto time_idx = _ad_opcs.find_in_time(ad.attrs());
                for (auto &idx: time_idx) {
                    auto &ctx = _ad_opcs.time_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad(id={}) time fail: {}", i, ex.what());
                success = false;
            }

            if (ad_feats->disc_feats_size() > 0 || ad_feats->cont_feats_size() > 0) {
                outp_feats.mutable_ads()->AddAllocated(ad_feats);
            }
        }

        return success;
    }

    bool AttrTranser::apply_ads_with_multi_thread(const AUCImprAttrs &inp_attrs, AUCImprFeats &outp_feats,
                                                  google::protobuf::Arena &arena) {
        bool success = true;
        std::mutex w_mutex;

        boost::latch count_down_latch(inp_attrs.ads_size());

        std::vector<std::pair<int, FeatureBunch *>> outp_;
        outp_.reserve(inp_attrs.ads_size());

        auto thread_func = [this, &inp_attrs, &arena, &count_down_latch, &w_mutex, &success, &outp_](int id,
                                                                                                     int index) {
            auto &ad = inp_attrs.ads(index);
            auto *ad_feats = google::protobuf::Arena::CreateMessage<FeatureBunch>(&arena);

            try {
                // Find the candidate attribute operation contexts
                auto cate_idx = _ad_opcs.find_in_cate(ad.attrs());
                for (auto &idx: cate_idx) {
                    auto &ctx = _ad_opcs.cate_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cate fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto setcate_idx = _ad_opcs.find_in_setcate(ad.attrs());
                for (auto &idx: setcate_idx) {
                    auto &ctx = _ad_opcs.setcate_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad (id={}) cate fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto rv_idx = _ad_opcs.find_in_rv(ad.attrs());
                for (auto &idx: rv_idx) {
                    auto &ctx = _ad_opcs.rv_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad(id={}) rv fail: {}", index, ex.what());
                success = false;
            }

            try {
                // Find the candidate attribute operation contexts
                auto time_idx = _ad_opcs.find_in_time(ad.attrs());
                for (auto &idx: time_idx) {
                    auto &ctx = _ad_opcs.time_ctx[idx];
                    // For every candidate context, apply its transform
                    if (!ctx->apply(ad, *ad_feats, arena)) {
                    }
                }
            } catch (const exception &ex) {
                _logger->warn("Transform on ad(id={}) time fail: {}", index, ex.what());
                success = false;
            }

            {
                std::lock_guard<std::mutex> lock(w_mutex);
                outp_.emplace_back(std::move(make_pair(index, ad_feats)));
            }

            count_down_latch.count_down();
        };

        for (int i = 0; i < inp_attrs.ads_size(); ++i) {
            _thp_rsrc->push(thread_func, i);
        }
        count_down_latch.wait();

        // sort
        std::sort(outp_.begin(), outp_.end(),
                  [](const std::pair<int, FeatureBunch *> &a, const std::pair<int, FeatureBunch *> &b) {
                      return a.first < b.first;
                  });
        for_each(outp_.begin(), outp_.end(), [&outp_feats](const pair<int, FeatureBunch *> &v) {
            if (v.second->disc_feats_size() > 0 || v.second->cont_feats_size() > 0) {
                outp_feats.mutable_ads()->AddAllocated(v.second);
            }
        });

        return success;
    }

    bool AttrTranser::apply_labels(const ::google::protobuf::RepeatedField<float> &attr_labels,
                                   ::google::protobuf::RepeatedField<float> &feat_labels) {
        bool success = true;

        try {
            // Copy the labels from attribute data to feature data
            for (int i = 0; i < attr_labels.size(); ++i) {
                feat_labels.Add(attr_labels.Get(i));
            }
        } catch (const exception &ex) {
            success = false;
            _logger->error("Copy attribute labels to feature labels fail: {}", ex.what());
        }

        return success;
    }

}
