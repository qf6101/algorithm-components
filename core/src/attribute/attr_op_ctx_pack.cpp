//
// Created by qfeng on 2019-01-20.
//

#include <attribute/attr_op_ctx_pack.h>
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

namespace algocomp {

    bool AttrOpCtxPack::self_load() {
        try {
            // Initialize index map of categorical attribute operation context
            for (int i = 0; i < cate_ctx.size(); ++i) {
                cate_idx[cate_ctx[i]->_in_aname] = i;
            }

            // Initialize index map of real valued attribute operation context
            for (int i = 0; i < rv_ctx.size(); ++i) {
                rv_idx[rv_ctx[i]->_in_aname] = i;
            }

            // Initialize index map of time attribute operation context
            for (int i = 0; i < time_ctx.size(); ++i) {
                time_idx[time_ctx[i]->_in_aname] = i;
            }

            // Initialize index map of setcate attribute operation context
            for (int i = 0; i < setcate_ctx.size(); ++i) {
                setcate_idx[setcate_ctx[i]->_in_aname] = i;
            }

            // Finish initializations
            return true;
        } catch (const exception &ex) {
            _logger->error("Load attribute opctx pack fail: {}", ex.what());
        }

        return false;
    }

    vector<int> AttrOpCtxPack::find_in_cate(
            const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const {
        vector<int> outp;
        outp.reserve(inp_attrs.size());

        for (auto &attr_: inp_attrs) {
            auto attr = attr_.second;

            // Match categorical attribute operation context with input attribute name
            auto found = cate_idx.find(attr_.first);
            if (found != cate_idx.end()) {

                // Skip unsupported data type of categorical attribute
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kFloatVal ||
                    attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kScateVal)
                    continue;

                // Skip empty string value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal &&
                    trim_copy(attr.str_val()).length() == 0)
                    continue;

                // Add the index of matched categorical attribute operation context
                outp.push_back(found->second);
            }
        }

        return outp;
    }

    vector<int> AttrOpCtxPack::find_in_rv(
            const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const {
        vector<int> outp;
        outp.reserve(inp_attrs.size());

        for (auto &attr_: inp_attrs) {
            auto attr = attr_.second;

            // Match real valued attribute operation context with input attribute name
            auto found = rv_idx.find(attr_.first);
            if (found != rv_idx.end()) {

                // Skip unsupported data type of real valued attribute
                if (attr.oneof_attr_val_case() != AttributeData::OneofAttrValCase::kFloatVal &&
                    attr.oneof_attr_val_case() != AttributeData::OneofAttrValCase::kIntVal &&
                    attr.oneof_attr_val_case() != AttributeData::OneofAttrValCase::kStrVal)
                    continue;

                // Skip zero value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kFloatVal && attr.float_val() == 0)
                    continue;

                // Skip zero value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kIntVal && attr.int_val() == 0)
                    continue;

                // Skip empty string value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal &&
                    trim_copy(attr.str_val()).length() == 0)
                    continue;

                // Add the index of matched real valued attribute operation context
                outp.push_back(found->second);
            }
        }

        return outp;
    }

    vector<int> AttrOpCtxPack::find_in_time(
            const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const {
        vector<int> outp;
        outp.reserve(inp_attrs.size());

        for (auto &attr_: inp_attrs) {
            auto attr = attr_.second;

            // Match time attribute operation context with input attribute name
            auto found = time_idx.find(attr_.first);
            if (found != time_idx.end()) {

                // Skip unsupported data type of time attribute
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kFloatVal ||
                    attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kScateVal)
                    continue;

                // Skip too small value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kIntVal && attr.int_val() < 100)
                    continue;

                // Skip too small value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kLongVal && attr.long_val() < 100)
                    continue;

                // Skip empty string value
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal &&
                    trim_copy(attr.str_val()).length() == 0)
                    continue;

                // Add the index of matched time attribute operation context
                outp.push_back(found->second);
            }
        }

        return outp;
    }

    vector<int> AttrOpCtxPack::find_in_setcate(
            const ::google::protobuf::Map<::std::string, ::algocomp::AttributeData> &inp_attrs) const {
        vector<int> outp;
        outp.reserve(inp_attrs.size());

        for (auto &attr_: inp_attrs) {
            auto attr = attr_.second;

            // Match set-categorical attribute operation context with input attribute name
            auto found = setcate_idx.find(attr_.first);
            if (found != setcate_idx.end()) {

                // Skip unsupported data type of set-categorical attribute
                if (attr.oneof_attr_val_case() != AttributeData::OneofAttrValCase::kScateVal &&
                    attr.oneof_attr_val_case() != AttributeData::OneofAttrValCase::kStrVal)
                    continue;

                // Add the index of matched set-categorical attribute operation context
                outp.push_back(found->second);
            }
        }

        return outp;
    }
}

