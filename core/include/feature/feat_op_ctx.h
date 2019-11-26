//
// Created by qfeng on 2019-01-04.
//

#ifndef ALGORITHM_COMPONENTS_FEAT_OP_CTX_H
#define ALGORITHM_COMPONENTS_FEAT_OP_CTX_H

#include <string>
#include <memory>
#include "feature/feature_operator.h"
#include "featpb/disc_feat_grp.pb.h"
#include "featpb/cont_feat_grp.pb.h"
#include "log/algocomp_log.h"

using namespace std;

namespace algocomp {

    // Declare referenced class
    class FeatOpCtxPack;

    // Define operation scope (match on feature group name or feature name)
    enum class OP_SCOPE {
        UNKNOWN = 0,
        GNAME = 1, // Match on feature group name
        FNAME = 2 // Match on feature name
    };

    /**
     * Feature operation context
     *
     * Define context of transform from feature to feature cyclically. The names of input feature (feature group name
     * and feature name) are specified in advance. They will be used for matching in runtime. The output names or the
     * generation pattern of output names are also predefined. This is merely an abstract class. The specific behavior
     * (i.e., "parse_procs_and_params") is defined in concrete classes (e.g., UnaryOpCtx and BinaryOpCtx).
     *
     */
    class FeatOpCtx {
    protected:
        // Define generation pattern of output feature group name
        enum class _GEN_OUT_GNAME {
            UNKNOWN = 0,
            EX_IN_FNAME_WITH_OP = 1, // Extend the input feature name with operator name
            CONCAT_IN_GNAMES = 2 // Concatenate the input feature group names (for binary operator)
        };

        // Define generation pattern of output feature name
        enum class _GEN_OUT_FNAME {
            UNKNOWN = 0,
            EQ_OUT_GNAME = 1, // The same as the output feature group name
            EX_OUT_GNAME_WITH_VAL = 2, // Extend the output feature group name with feature value
            CONCAT_IN_FNAMES = 3 // Concatenate the input feature names (for binary operator)
        };

        // Associated operator name
        const string _op_name;

        // Operation scope (match on feature group name or feature name)
        OP_SCOPE _op_scope = OP_SCOPE::UNKNOWN;

        // Generation pattern of output feature group name
        _GEN_OUT_GNAME _gen_out_gname = _GEN_OUT_GNAME::UNKNOWN;

        // Generation pattern of output feature name
        _GEN_OUT_FNAME _gen_out_fname = _GEN_OUT_FNAME::UNKNOWN;

        // Parse patterns of "processes" together with their "parameters". This function is used in "parse_patterns"
        // function and is explained differently by concrete classes. The "processes" define the computations on the
        // input feature value to generate the output feature value.
        virtual bool parse_procs_and_params(const string &proc_desc, const string &param_desc) = 0;

    public:
        // Constructor
        FeatOpCtx(const string op_name) : _op_name(op_name) {};

        // Destructor
        virtual ~FeatOpCtx() = default;

        // Get the operator scope
        const OP_SCOPE op_scope() const { return _op_scope; }
    };

    /**
     * Unary feature operation context
     *
     * Define context of unary transform from feature to feature (e.g., input feature group name and feature name).
     */
    class UnaryOpCtx : public FeatOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    protected:
        // Declare friend class
        friend FeatOpCtxPack;

        // Names of input feature (feature group name and feature name)
        string _in_gname = "";
        string _in_fname = "";

    public:
        // Constructor with logger
        UnaryOpCtx(const string op_name) : FeatOpCtx(op_name) { _logger = spdlog::get(ACLog::_logger_name); }

        // Compute the names of output feature according to predefined generation patterns in runtime
        const pair<string, string> out_names(const string &in_gname, const string &in_fname, const int in_fval) const;

        // Parse all patterns from configured strings
        const bool parse_patterns(const string in_gname, const string in_fname, const string out_gname,
                                  const string out_fname, const string op_scope, const string proc_desc,
                                  const string param_desc);

        // Destructor
        virtual ~UnaryOpCtx() = default;

    };

    /**
     * Binary feature operation context
     *
     * Define context of binary transform from feature to feature (e.g., input names of features).
     */
    class BinaryOpCtx : public FeatOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;
    protected:
        // Declare friend class
        friend FeatOpCtxPack;

        // Names of input features (feature group name and feature name)
        string _in_gname1 = "";
        string _in_fname1 = "";
        string _in_gname2 = "";
        string _in_fname2 = "";
    public:
        // Constructor with logger
        BinaryOpCtx(const string op_name) : FeatOpCtx(op_name) { _logger = spdlog::get(ACLog::_logger_name); }

        // Compute the names of output feature according to predefined generation patterns in runtime
        const pair<string, string>
        out_names(const string &in_gname1, const string &in_fname1, const string &in_gname2, const string &in_fname2,
                  const string &suffix = "", const bool double_binary = false, const int in_fval = 0) const;

        // Parse all patterns from configured strings
        const bool parse_patterns(const string in_gname1, const string in_fname1, const string in_gname2,
                                  const string in_fname2, const string out_gname, const string out_fname,
                                  const string op_scope, const string proc_desc, const string param_desc);

        // Destructor
        virtual ~BinaryOpCtx() = default;

    };

    /**
     * Continuous to continuous unary feature operation context
     *
     * Define context of unary transform from continuous feature to continuous feature.
     *
     */
    class Cont2ContUnaryOpCtx : public UnaryOpCtx {
    public:
        // Constructor
        Cont2ContUnaryOpCtx(const string op_name) : UnaryOpCtx(op_name) {}

        // Apply transform from continuous feature to continuous feature in runtime
        virtual optional<pair<string, ContFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const = 0;

        // Destructor
        virtual ~Cont2ContUnaryOpCtx() = default;
    };

    /**
     * Continuous to continuous binary feature operation context
     *
     * Define context of binary transform from continuous feature to continuous feature.
     *
     */
    class Cont2ContBinaryOpCtx : public BinaryOpCtx {
    public:
        // Constructor
        Cont2ContBinaryOpCtx(const string op_name) : BinaryOpCtx(op_name) {}

        // Apply transform from continuous feature to continuous feature in runtime
        virtual optional<pair<string, ContFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const = 0;

        // Destructor
        virtual ~Cont2ContBinaryOpCtx() = default;
    };

    /**
     * Continuous to discrete unary feature operation context
     *
     * Define context of unary transform from continuous feature to discrete feature.
     *
     */
    class Cont2DiscUnaryOpCtx : public UnaryOpCtx {
    public:
        // Constructor
        Cont2DiscUnaryOpCtx(const string op_name) : UnaryOpCtx(op_name) {}

        // Apply transform from continuous feature to discrete feature in runtime
        virtual optional<pair<string, DiscFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, ContFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const = 0;

        // Destructor
        virtual ~Cont2DiscUnaryOpCtx() = default;
    };

    /**
     * Continuous to continuous binary feature operation context
     *
     * Define context of binary transform from continuous feature to continuous feature.
     *
     */
    class Disc2DiscBinaryOpCtx : public BinaryOpCtx {
    protected:
        // Batch operation indicator
        bool _batch = false;
    public:
        // Constructor
        Disc2DiscBinaryOpCtx(const string op_name) : BinaryOpCtx(op_name) {}

        // Check if it is a batch operation (e.g., crossall operation)
        bool batch() { return _batch; }

        // Apply transform from discrete feature to discrete feature in runtime
        virtual optional<pair<string, DiscFeatGrp>>
        apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
              google::protobuf::Arena &arena) const = 0;

        // Batch apply transform from discrete features to discrete features in runtime (e.g., crossall operation)
        virtual optional<map<string, DiscFeatGrp>>
        batch_apply(const vector<::google::protobuf::Map<string, DiscFeatGrp>> &inp_feats,
                    google::protobuf::Arena &arena) const = 0;

        // Destructor
        virtual ~Disc2DiscBinaryOpCtx() = default;

    };

}


#endif //ALGORITHM_COMPONENTS_FEAT_OP_CTX_H
