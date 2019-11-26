//
// Created by qfeng on 2019-01-15.
//

#ifndef ALGORITHM_COMPONENTS_TIME_ATTR_OP_CTX_H
#define ALGORITHM_COMPONENTS_TIME_ATTR_OP_CTX_H


#include "attr_op_ctx.h"
#include "featpb/feature_bunch.pb.h"
#include <vector>
#include <limits>
#include <spdlog/logger.h>


namespace algocomp {

    class TimeAttrOpCtx: public AttrOpCtx {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

        // Define time transform levels
        bool _hour_level = false;
        bool _day_level = false;
        bool _month_level = false;
        bool _week_level = false;

        // Time locale for string format time
        std::locale _time_loc;

    public:
        // Constructor
        TimeAttrOpCtx();

        // Parse processes together with their parameters for computing feature value from attribute value
        bool parse_procs_and_params(const string &proc_desc, const string &param_desc) override;

        // Apply transform from time attribute to a set of discrete features in runtime
        bool apply(const AttributeBunch &inp_attrs, FeatureBunch &outp_feats, google::protobuf::Arena &arena) override;

        // Unix time limit (utc of 2100/01/01)
        static const unsigned long TIME_LIMIT;
    };

}


#endif //ALGORITHM_COMPONENTS_TIME_ATTR_OP_CTX_H
