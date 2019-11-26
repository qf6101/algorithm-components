//
// Created by qfeng on 2019-01-08.
//

#ifndef ALGORITHM_COMPONENTS_ATTR_OP_CTX_PACK_H
#define ALGORITHM_COMPONENTS_ATTR_OP_CTX_PACK_H

#include <memory>
#include <vector>
#include "cate_attr_op_ctx.h"
#include "rv_attr_op_ctx.h"
#include "time_attr_op_ctx.h"
#include "setcate_attr_op_ctx.h"
#include "dense_hash_map"

using namespace std;

namespace algocomp {

    /**
     * Package of attribute operation contexts
     *
     * The container of attribute operation contexts for either user-and-context or ad.
     */
    class AttrOpCtxPack {
    private:
        // Spdlog logger
        shared_ptr<spdlog::logger> _logger;

    public:
        // Constructor with logger and indices of attribute operation context
        AttrOpCtxPack() {
            _logger = spdlog::get(ACLog::_logger_name);
            cate_idx.set_empty_key("");
            rv_idx.set_empty_key("");
            time_idx.set_empty_key("");
            setcate_idx.set_empty_key("");
        }

        // Attribute operation contexts
        vector<shared_ptr<CateAttrOpCtx>> cate_ctx;
        vector<shared_ptr<RVAttrOpCtx>> rv_ctx;
        vector<shared_ptr<TimeAttrOpCtx>> time_ctx;
        vector<shared_ptr<SetCateAttrOpCtx>> setcate_ctx;

        // Index map of attribute operation contexts (used for fast matching with input attribute names)
        google::dense_hash_map<string, int> cate_idx;
        google::dense_hash_map<string, int> rv_idx;
        google::dense_hash_map<string, int> time_idx;
        google::dense_hash_map<string, int> setcate_idx;

        // Self load the index map of attribute operation contexts
        bool self_load();

        // Match categorical attribute operation contexts with input attribute names and find their indices
        vector<int>
        find_in_cate(const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const;

        // Match real valued attribute operation contexts with input attribute names and find their indices
        vector<int>
        find_in_rv(const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const;

        // Match time attribute operation contexts with input attribute names and return find indices
        vector<int>
        find_in_time(const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const;

        // Match setcate attribute operation contexts with input attribute names and return find indices
        vector<int>
        find_in_setcate(const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const;

    };

}


#endif //ALGORITHM_COMPONENTS_ATTR_OP_CTX_PACK_H
