//
// Created by qfeng on 2018-12-26.
//

#ifndef ALGOCOMP_ATTRIBUTE_TRANSFORMER_H
#define ALGOCOMP_ATTRIBUTE_TRANSFORMER_H

#include <google/protobuf/message.h>
#include <vector>
#include "attrpb/auc_impr_attrs.pb.h"
#include "featpb/auc_impr_feats.pb.h"
#include "attr_op_ctx_pack.h"
#include "log/algocomp_log.h"
#include <boost/property_tree/ptree.hpp>
#include "ctpl.h"
#include <common/type_def.h>

using namespace std;
namespace pt = boost::property_tree;

namespace algocomp {

    // Declare referenced class
    class ResourceManager;

    /**
     * Attribute transformer
     *
     * Define transforms from input attribute data to output feature data for both user-and-context and ad.
     * Attribute data consists of a set attributes and feature data consists of a set features.
     * This attribute transformer applies the predefined transforms to input attribute data in runtime.
     */
    class AttrTranser {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Thread pool resource
        shared_ptr<ctpl::thread_pool> _thp_rsrc;

        // Thread type
        ThreadType _thread_type;

        // Packages of attribute operation contexts for user-and-context and ad
        AttrOpCtxPack _uc_opcs;
        AttrOpCtxPack _ad_opcs;

        // Signal to outer
        bool _started = false;

        // Apply predefined transforms to user-and-context attributes
        bool apply_uc(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats, google::protobuf::Arena &arena);

        // Apply predefined transforms to user-and-context attributes with mult-thread
        bool apply_uc_with_multi_thread(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats,
                                        google::protobuf::Arena &arena);

        // Apply predefined transforms to ad attributes
        bool apply_ads(const AUCImprAttrs &inp_attrs, AUCImprFeats &outp_feats, google::protobuf::Arena &arena);

        // Apply predefined transforms to ad attributes with multi-thread
        bool apply_ads_with_multi_thread(const AUCImprAttrs &inp_attrs, AUCImprFeats &outp_feats,
                                         google::protobuf::Arena &arena);

        // Copy labels from attribute data to feature data
        bool apply_labels(const ::google::protobuf::RepeatedField<float> &attr_labels,
                          ::google::protobuf::RepeatedField<float> &feat_labels);

        // Create categorical attribute operation contexts in initialization
        bool create_cate_opctx(const pt::ptree &ctx_item, vector<shared_ptr<CateAttrOpCtx>> &cate_ctx);

        // Create real valued attribute operation contexts in initialization
        bool create_rv_opctx(const pt::ptree &ctx_item, vector<shared_ptr<RVAttrOpCtx>> &rv_ctx);

        // Create time attribute operation contexts in initialization
        bool create_time_opctx(const pt::ptree &ctx_item, vector<shared_ptr<TimeAttrOpCtx>> &time_ctx);

        // Create setcate attribute operation contexts in initialization
        bool create_setcate_opctx(const pt::ptree &ctx_item, vector<shared_ptr<SetCateAttrOpCtx>> &setcate_ctx);

        // Parse configurations of categorical attributes
        bool parse_cate_patterns(const pt::ptree &ctx_item, shared_ptr<CateAttrOpCtx> &ctx);

        // Parse configurations of real valued attributes
        bool parse_rv_patterns(const pt::ptree &ctx_item, shared_ptr<RVAttrOpCtx> &ctx);

        // Parse configurations of time attributes
        bool parse_time_patterns(const pt::ptree &ctx_item, shared_ptr<TimeAttrOpCtx> &ctx);

        // Parse configurations of setcate attributes
        bool parse_setcate_patterns(const pt::ptree &ctx_item, shared_ptr<SetCateAttrOpCtx> &ctx);

    public:
        // Declare friend class
        friend ResourceManager;

        // Constructor with initializations
        AttrTranser(const std::string &conf_fname, const std::string &conf_dname,
                    shared_ptr<ctpl::thread_pool> thp_rsrc,
                    const ThreadType _thread_type = ThreadType::NO);

        // Apply predefined transforms to input attribute data in runtime
        optional<AUCImprFeats> apply(const AUCImprAttrs &inp_attrs, google::protobuf::Arena &arena);

        // Signal to outer
        bool Started() { return _started; }

    };

}


#endif //ALGOCOMP_ATTRIBUTE_TRANSFORMER_H
