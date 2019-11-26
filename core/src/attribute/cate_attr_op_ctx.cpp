//
// Created by qfeng on 2019-01-08.
//

#include "attribute/cate_attr_op_ctx.h"
#include "absl/strings/str_cat.h"
#include <absl/strings/str_split.h>
#include "log/algocomp_log.h"
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

namespace algocomp {

    // Define static object for forbidden char checking
    static CateAttrOpCtx __cate_attr_op_ctx;

    // Free function for forbidden char checking (used with std::replace_if)
//    bool __is_forbidden_char(const char &ch) {
//        return __cate_attr_op_ctx.contains_forbidden_chars(ch);
//    }

    CateAttrOpCtx::CateAttrOpCtx() {
        // Define Spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
        // Define forbidden chars (Vowpal Wabbit and Mongodb need)
        _forbidden_chars.set_empty_key('a');
        _forbidden_chars.insert(' ');
        _forbidden_chars.insert('|');
        _forbidden_chars.insert(':');
        _forbidden_chars.insert('\n');
        _forbidden_chars.insert('\t');
        _forbidden_chars.insert('.');
        _forbidden_chars.insert('$');
    }

    bool CateAttrOpCtx::contains_forbidden_chars(const char &ch) {
        return _forbidden_chars.find(ch) != _forbidden_chars.end();
    }

    bool CateAttrOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "" && param_desc == "") {
                _logger->warn("Empty procs and params for categorical attribute operation context (in_aname={}).",
                              _in_aname);
                return true;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error("Parsed inconsistent sizes of procs {} and params {} (in_aname={})", proc_desc,
                               param_desc, _in_aname);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);
                auto param_name = trim_copy(param_splits[i]);


                if (proc_name == "prune_max_len") { // Parse process of pruning maximum length
                    std::vector<std::string> rule_and_maxlen = absl::StrSplit(param_name, ',');

                    // Parse parameter of rule
                    if (trim_copy(rule_and_maxlen[0]) == "skip") { // Skip the attribute
                        _exceed_rule = _EXCEED_RULE::SKIP;
                    } else if (trim_copy(rule_and_maxlen[0]) == "cut_head") { // Cut from head to maximum length
                        _exceed_rule = _EXCEED_RULE::CUT_HEAD;
                    } else if (trim_copy(rule_and_maxlen[0]) == "cut_tail") { // Cut to tail with maximum length
                        _exceed_rule = _EXCEED_RULE::CUT_TAIL;
                    } else { // Wrong parameter
                        _logger->error("Configured wrong max length rule {} (in_aname={}).", rule_and_maxlen[0],
                                       _in_aname);
                        return false;
                    }

                    // Parse parameter of maximum length
                    int max_len = std::stoi(trim_copy(rule_and_maxlen[1]));
                    if (max_len < 0 && max_len > _max_len) {
                        _logger->warn(
                                "Max length of {} of categorical attribute is {} which is not in proper range, will be reset (in_aname={}).",
                                proc_splits[i], max_len, _in_aname);
                        _max_len = max_len;
                    }
                } else { // Wrong process
                    _logger->error("Parsed unknown categorical attribute proc {} (in_aname={})", proc_splits[i],
                                   _in_aname);
                }
            }

            return true;

        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong procs and params (proc_desc={}, param_desc={}) for categorical attribute (in_aname={}): {}.",
                    proc_desc, param_desc, _in_aname, ex.what());
        }

        return false;
    }


    bool CateAttrOpCtx::apply(const algocomp::AttributeBunch &inp_attrs, algocomp::FeatureBunch &outp_feats,
                              google::protobuf::Arena &arena) {

        try {
            // Match input attribute
            auto found = inp_attrs.attrs().find(_in_aname);
            if (found != inp_attrs.attrs().end()) {
                auto attr = (*found).second;

                // Parse attribute value with different data type
                string fval_str = "";
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kIntVal) {
                    fval_str = std::to_string(attr.int_val());
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal) {
                    fval_str = attr.str_val();
                    auto pred = [&](const char &ch){
                        return algocomp::__is_forbidden_char<CateAttrOpCtx>(__cate_attr_op_ctx, ch);
                    };
                    std::replace_if(fval_str.begin(), fval_str.end(), pred, '-');
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kLongVal) {
                    fval_str = std::to_string(attr.long_val());
                } else {
                    _logger->warn(
                            "Categorical attribute (in_aname={}, out_gname={}) got neither int/long value nor string value but {}",
                            _in_aname, _out_gname, attr.oneof_attr_val_case());
                    return false;
                }

                if (fval_str != "") {
                    // Treat too long attribute value
                    if (fval_str.length() > _max_len) {
                        if (_exceed_rule == _EXCEED_RULE::SKIP) {
                            _logger->debug(
                                    "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be discarded.",
                                    _in_aname, _out_gname, fval_str);
                            return false;
                        } else if (_exceed_rule == _EXCEED_RULE::CUT_HEAD) {
                            _logger->debug(
                                    "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be cut from head.",
                                    _in_aname, _out_gname, fval_str);
                            fval_str = fval_str.substr(0, _max_len);
                        } else if (_exceed_rule == _EXCEED_RULE::CUT_TAIL) {
                            _logger->debug(
                                    "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be cut from tail.",
                                    _in_aname, _out_gname, fval_str);
                            fval_str = fval_str.substr(fval_str.length() - _max_len);
                        }
                    }

                    // Create output feature
                    string out_fname = absl::StrCat(_out_gname, "%", fval_str);
                    DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                    DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                    feat->set_gname(_out_gname);
                    feat->set_fname(out_fname);
                    (*feat_grp->mutable_feats())[out_fname] = *feat;
                    (*outp_feats.mutable_disc_feats())[_out_gname] = *feat_grp;
                } else {
                    _logger->debug("Out_fname (in_aname={}, out_gname={}) is empty which maybe not set properly.",
                                   _in_aname, _out_gname);
                    return false;
                }
            }
            // Everything is OK
            return true;

        } catch (const exception &ex) {
            _logger->error("Operated on wrong categorical attribute data: {}.", ex.what());
        }
        return false;
    }
}
