//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_ATTR_OP_CTX_H
#define ALGORITHM_COMPONENTS_ATTR_OP_CTX_H

#include "attrpb/attribute_bunch.pb.h"
#include "log/algocomp_log.h"
#include <string>

using namespace std;

namespace algocomp {

    // Free function for forbidden char checking (used with std::replace_if)
    template <typename T>
    bool __is_forbidden_char(T &t, const char &ch) {
        return t.contains_forbidden_chars(ch);
    }

    // Declare referenced classes
    class AttrOpCtxPack;

    class FeatureBunch;

    /**
     *  Attribute operation context
     *
     *  Define transformation from attribute to feature. The name of input attribute is specified in advance and
     *  will be used for matching in runtime. The output feature group and the generation pattern of feature name
     *  are also predefined. This is merely an abstract class. The specific behaviors (i.e., "parse_procs_and_params"
     *  function and "apply" function) are defined in the concrete classes (e.g., CateAttrOpCtx and RVAttrOpCtx).
     */
    class AttrOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    protected:
        // Declare friend class
        friend AttrOpCtxPack;

        // Input attribute name for matching in runtime
        string _in_aname = "";
        // Output feature group name
        string _out_gname = "";

        // Define generation patterns of feature name
        enum class _GEN_OUT_FNAME {
            UNKNOWN = 0,
            EQ_OUT_GNAME = 1, // The same as feature group name
            EX_OUT_GNAME_WITH_VAL = 2 // Extend the feature group name with feature value
        };

        // Generation pattern of output feature name
        _GEN_OUT_FNAME _gen_out_fname = _GEN_OUT_FNAME::UNKNOWN;

    public:
        // Constructor with logger
        AttrOpCtx() { _logger = spdlog::get(ACLog::_logger_name); };

        // Destructor
        virtual ~AttrOpCtx() = default;

        // Parse all patterns from configured strings
        bool parse_patterns(const string in_aname, const string out_gname, const string out_fname,
                            const string proc_desc, const string param_desc);

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input attribute value to generate the output feature value.
        virtual bool parse_procs_and_params(const string &proc_desc, const string &param_desc) = 0;

        // Apply the transformation from attribute to feature in runtime
        virtual bool
        apply(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats, google::protobuf::Arena &arena) = 0;
    };

}


#endif //ALGORITHM_COMPONENTS_ATTR_OP_CTX_H
