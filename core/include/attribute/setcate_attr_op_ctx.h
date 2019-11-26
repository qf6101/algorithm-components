//
// Created by AIYOJ on 2019-02-11.
//

#ifndef ALGORITHM_COMPONENTS_SETCATE_ATTR_OP_CTX_H
#define ALGORITHM_COMPONENTS_SETCATE_ATTR_OP_CTX_H

#include "attr_op_ctx.h"
#include "featpb/feature_bunch.pb.h"
#include <vector>
#include <spdlog/logger.h>
#include <dense_hash_set>


namespace algocomp {

    /**
     * Set-Categorical attribute operation context
     *
     * Define the transformation from set-categorical attribute to continuous feature.
     */
    class SetCateAttrOpCtx : public AttrOpCtx {
    private:
        // spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Define patterns of action for too long attribute value
        // like "name1:value1;name2:value2"
        // proc name1 and proc name2 by using _EXCEED_RULE
        enum class _EXCEED_RULE {
            SKIP = 1,
            CUT_HEAD = 2,
            CUT_TAIL = 3
        };


        // Action for too long attribute value (treated as string) while processing name1 and name2
        _EXCEED_RULE _exceed_rule = _EXCEED_RULE::SKIP;

        // Define maximum length of attribute value
        int _max_len = 200;

        // like "name1:value1;name2:value2"
        // Define forbidden chars (Vowpal Wabbit and Mongodb need) for name1 and name2
        google::dense_hash_set<char> _forbidden_chars;

        // Define pattern of value scale action
        enum class _NORM_TYPE {
            NONE,
            NORM_SUM = 1,
            USE_ONE = 2
        };

        // Value normalization
        _NORM_TYPE _norm_type = _NORM_TYPE::NONE;

        // key separator
        string _key_sep = "";

        string _val_sep = "";

    public:
        // Constructor
        SetCateAttrOpCtx();

        // Check if the input is a forbidden char (Vowpal Wabbit and Mongodb need)
        bool contains_forbidden_chars(const char &ch);

        // Parse processes together with their parameters for computing feature value from attribute value
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Apply transform from categorical attribute to discrete feature in runtime
        bool apply(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats, google::protobuf::Arena &arena) override;

    };
}

#endif //ALGORITHM_COMPONENTS_SETCATE_ATTR_OP_CTX_H
