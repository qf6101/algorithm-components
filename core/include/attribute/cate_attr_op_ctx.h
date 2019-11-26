//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_CATE_ATTR_OP_CTX_H
#define ALGORITHM_COMPONENTS_CATE_ATTR_OP_CTX_H

#include "attr_op_ctx.h"
#include "featpb/feature_bunch.pb.h"
#include <vector>
#include <spdlog/logger.h>
#include <dense_hash_set>

using namespace std;

namespace algocomp {

    /**
     * Categorical attribute operation context
     *
     * Define the transformation from categorical attribute to discrete feature.
     */
    class CateAttrOpCtx : public AttrOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Define patterns of action for too long attribute value
        enum class _EXCEED_RULE {
            SKIP = 1,
            CUT_HEAD = 2,
            CUT_TAIL = 3
        };

        // Action for too long attribute value (treated as string)
        _EXCEED_RULE _exceed_rule = _EXCEED_RULE::SKIP;

        // Define maximum length of attribute value
        int _max_len = 200;

        // Define forbidden chars (Vowpal Wabbit and Mongodb need)
        google::dense_hash_set<char> _forbidden_chars;

    public:
        // Constructor
        CateAttrOpCtx();

        // Check if the input is a forbidden char (Vowpal Wabbit and Mongodb need)
        bool contains_forbidden_chars(const char &ch);

        // Parse processes together with their parameters for computing feature value from attribute value
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Apply transform from categorical attribute to discrete feature in runtime
        bool apply(const AttributeBunch &inp_attrs, FeatureBunch& outp_feats, google::protobuf::Arena &arena) override;
    };

}


#endif //ALGORITHM_COMPONENTS_CATE_ATTR_OP_CTX_H
