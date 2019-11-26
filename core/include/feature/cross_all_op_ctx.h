//
// Created by qfeng on 2019-01-18.
//

#ifndef ALGORITHM_COMPONENTS_CROSS_ALL_OP_CTX_H
#define ALGORITHM_COMPONENTS_CROSS_ALL_OP_CTX_H

#include "feature/feat_op_ctx.h"
#include "featpb/disc_feat_grp.pb.h"
#include <optional>
#include <string>
#include <log/algocomp_log.h>
#include "feature/cross_op_ctx.h"
#include "dense_hash_set"

using namespace std;

namespace algocomp {

    // Define type of cross-all operation
    enum CrossAllOpType {
        UNKNOWN = 0,
        UC_INNER = 1, // Cross features within user-and-context
        AD_INNER = 2, // Cross features within ad
        AD2UC = 3 // Cross features between ad and user-and-context
    };

    /**
     * Cross-all operation context
     *
     * Define cross-all transform from discrete features to discrete feature
     */
    class CrossAllOpCtx : public CrossOpCtx {
    private:
        // Type of cross-all operation
        CrossAllOpType _cross_all_op_type = CrossAllOpType::UNKNOWN;

        // Ignore sets of feature group names and feature names for cross-all operation
        google::dense_hash_set<string> _ignore_uc_gnames;
        google::dense_hash_set<string> _ignore_uc_fnames;
        google::dense_hash_set<string> _ignore_ad_gnames;
        google::dense_hash_set<string> _ignore_ad_fnames;

        // Ignore cross-all transform on already crossed features
        bool _ignore_double_cross = false;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Implement the cross-all transform
        optional<map<string, DiscFeatGrp>>
        impl_crossing(const google::protobuf::Map<string, algocomp::DiscFeatGrp> &inp_lhs_feats,
                      const google::protobuf::Map<string, algocomp::DiscFeatGrp> &inp_rhs_feats,
                      const google::dense_hash_set<string> &ignore_lhs_gnames,
                      const google::dense_hash_set<string> &ignore_lhs_fnames,
                      const google::dense_hash_set<string> &ignore_rhs_gnames,
                      const google::dense_hash_set<string> &ignore_rhs_fnames,
                      google::protobuf::Arena &arena,
                      const string &how_cross) const;

    public:
        // Constructor
        CrossAllOpCtx(shared_ptr<CrossOp> &op) : CrossOpCtx(op, "crossall") {
            _logger = spdlog::get(ACLog::_logger_name);
            _batch = true;

            _ignore_uc_gnames.set_empty_key("");
            _ignore_uc_fnames.set_empty_key("");
            _ignore_ad_gnames.set_empty_key("");
            _ignore_ad_fnames.set_empty_key("");
        }

        // Apply cross-all transform from discrete features to discrete features in runtime
        optional<pair<string, DiscFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const override;

        // Apply batch cross-all transform from discrete features to discrete features in runtime
        optional<map<string, DiscFeatGrp>>
        batch_apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
                    google::protobuf::Arena &arena) const override;
    };

}


#endif //ALGORITHM_COMPONENTS_CROSS_ALL_OP_CTX_H
