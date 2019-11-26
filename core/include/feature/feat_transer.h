//
// Created by qfeng on 2018-12-26.
//

#ifndef ALGOCOMP_FEATURE_TRANSFORMER_H
#define ALGOCOMP_FEATURE_TRANSFORMER_H

#include <map>
#include <string>
#include <vector>
#include "featpb/auc_impr_feats.pb.h"
#include "feature/feat_op_ctx_pack.h"
#include "featpb/feature_bunch.pb.h"
#include <boost/property_tree/ptree.hpp>
#include "ctpl.h"
#include <spdlog/logger.h>
#include <common/type_def.h>


using namespace std;
namespace pt = boost::property_tree;

namespace algocomp {

    // Declare referenced class
    class ResourceManager;

    /**
     * Feature Transformer
     *
     * Define transforms from input feature data to output feature data for both user-and-context and ad.
     * Feature data consists of a set features. This feature transformer applies the predefined transforms
     * to input feature data in runtime.
     *
     */
    class FeatTranser {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Thread pool resource
        shared_ptr<ctpl::thread_pool> _thp_rsrc;

        // Thread type
        ThreadType _thread_type;

        // Packages of feature operation contexts for user-and-context and ad
        FeatOpCtxPack _uc_opcs;
        FeatOpCtxPack _ad_opcs;

        // Apply predefined transforms to user-and-context features
        bool apply_uc(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Apply predefined transforms to user-and-context features with multi-thread
        bool
        apply_uc_with_multi_thread(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Apply predefined transforms to ad features
        bool apply_ads(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Apply predefined transforms to ad features with multi-thread
        bool
        apply_ads_with_multi_thread(const FeatOpCtxPack &opcs, AUCImprFeats &inp_feats, google::protobuf::Arena &arena);

        // Create ccu feature operation contexts in initialization
        bool create_ccu_opctx(const pt::ptree &ctx_item, vector<shared_ptr<Cont2ContUnaryOpCtx>> &ccu_ctx,
                              map<string, shared_ptr<FeatureOperator>> &op_rsrc);

        // Create ccb feature operation contexts in initialization
        bool create_ccb_opctx(const pt::ptree &ctx_item, vector<shared_ptr<Cont2ContBinaryOpCtx>> &ccb_ctx,
                              map<string, shared_ptr<FeatureOperator>> &op_rsrc);

        // Create cdu feature operation contexts in initialization
        bool create_cdu_opctx(const pt::ptree &ctx_item, vector<shared_ptr<Cont2DiscUnaryOpCtx>> &cdu_ctx,
                              map<string, shared_ptr<FeatureOperator>> &op_rsrc);

        // Create ddb feature operation contexts in initialization
        bool create_ddb_opctx(const pt::ptree &ctx_item, vector<shared_ptr<Disc2DiscBinaryOpCtx>> &ddb_ctx,
                              map<string, shared_ptr<FeatureOperator>> &op_rsrc);

        // Parse configurations of unary patterns
        bool parse_unary_patterns(const pt::ptree &ctx_item, shared_ptr<UnaryOpCtx> &ctx);

        // Parse configurations of binary patterns
        bool parse_binary_patterns(const pt::ptree &ctx_item, shared_ptr<BinaryOpCtx> &ctx);

    public:
        // Constructor
        FeatTranser(const string &conf_fname, const string &conf_dname, shared_ptr<ctpl::thread_pool> thp_rsrc,
                    map<string, shared_ptr<algocomp::FeatureOperator>> &op_rsrc,
                    const ThreadType _thread_type = ThreadType::NO);

        // Apply predefined transforms to input feature data in runtime
        bool apply(AUCImprFeats &inp_feats, google::protobuf::Arena &arena);
    };
}


#endif //ALGOCOMP_FEATURE_TRANSFORMER_H
