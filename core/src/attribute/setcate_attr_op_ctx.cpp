//
// Created by AIYOJ on 2019-02-11.
// Modified by qfeng on 2019-02-22 (string format part).
//

#include "attribute/setcate_attr_op_ctx.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "log/algocomp_log.h"
#include <boost/algorithm/string.hpp>


namespace algocomp {

    static SetCateAttrOpCtx __setcate_attr_op_ctx;

    SetCateAttrOpCtx::SetCateAttrOpCtx() {
        _logger = spdlog::get(ACLog::_logger_name);
        _forbidden_chars.set_empty_key('a');
        _forbidden_chars.insert(' ');
        _forbidden_chars.insert('|');
        _forbidden_chars.insert(':');
        _forbidden_chars.insert('\n');
        _forbidden_chars.insert('\t');
        _forbidden_chars.insert('.');
        _forbidden_chars.insert('$');
    }

    bool SetCateAttrOpCtx::contains_forbidden_chars(const char &ch) {
        return _forbidden_chars.find(ch) != _forbidden_chars.end();
    }

    bool SetCateAttrOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            if (proc_desc == "" && param_desc == "") {
                _logger->warn("Empty procs and params for set-categorical attribute operation context (in_aname={}).",
                              _in_aname);

                return true;
            }

            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            if (proc_splits.size() != param_splits.size()) {
                _logger->error("Parsed inconsistent sizes of procs {} and params {} (in_aname={})", proc_desc,
                               param_desc, _in_aname);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = boost::algorithm::trim_copy(proc_splits[i]);
                auto param_name = boost::algorithm::trim_copy(param_splits[i]);

                if (proc_name == "norm_type") {
                    if (param_name == "norm_sum") _norm_type = _NORM_TYPE::NORM_SUM;
                    else if (param_name == "use_one") _norm_type = _NORM_TYPE ::USE_ONE;
                    else if (param_name == "none") _norm_type = _NORM_TYPE ::NONE;
                    else {
                        _logger->error("Parsed unknown norm type {}", param_name);
                        return false;
                    }
                } else if (proc_name == "prune_max_len") {
                    std::vector<std::string> rule_and_maxlen = absl::StrSplit(param_name, ',');

                    if (boost::algorithm::trim_copy(rule_and_maxlen[0]) == "skip") {
                        _exceed_rule = _EXCEED_RULE::SKIP;
                    } else if (boost::algorithm::trim_copy(rule_and_maxlen[0]) == "cut_head") {
                        _exceed_rule = _EXCEED_RULE::CUT_HEAD;
                    } else if (boost::algorithm::trim_copy(rule_and_maxlen[0]) == "cut_tail") {
                        _exceed_rule = _EXCEED_RULE::CUT_TAIL;
                    } else {
                        _logger->error("Configured wrong max length rule {} (in_aname={}).", rule_and_maxlen[0],
                                       _in_aname);
                        return false;
                    }

                    // Parse parameter of maximum length
                    int max_len = std::stoi(boost::algorithm::trim_copy(rule_and_maxlen[1]));
                    if (max_len < 0 && max_len > _max_len) {
                        _logger->warn(
                                "Max length of {} of categorical attribute is {} which is not in proper range, will be reset (in_aname={}).",
                                proc_splits[i], max_len, _in_aname);
                        _max_len = max_len;
                    }
                } else if (proc_name == "key_split") {
                    if (param_name == "comma") {
                        _key_sep = ",";
                    } else if (param_name == "semicolon") {
                        _key_sep = ";";
                    } else {
                        _logger->error("Parsed unsupported set-categorical key split param {}", param_name);
                        return false;
                    }
                } else if (proc_name == "val_split") {
                    if (param_name == "colon") {
                        _val_sep = ":";
                    } else {
                        _logger->error("Parsed unsupported set-categorical value split param {}", param_name);
                        return false;
                    }
                } else {
                    _logger->error("Parsed unknown set-categorical attribute proc {} (in_aname={})", proc_splits[i],
                                   _in_aname);
                    return false;
                }
            }

            return true;

        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong procs and params (proc_desc={}, param_desc={}) for set-categorical attribute (in_aname={}): {}.",
                    proc_desc, param_desc, _in_aname, ex.what());
        }

        return false;

    }

    bool SetCateAttrOpCtx::apply(const algocomp::AttributeBunch &inp_attrs, algocomp::FeatureBunch &outp_feats,
                                 google::protobuf::Arena &arena) {
        try {
            auto found = inp_attrs.attrs().find(_in_aname);
            if (found != inp_attrs.attrs().end()) {

                auto attr = (*found).second;

                map<string, float> kv;
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kScateVal) {
                    for (auto &setcate: attr.scate_val().vals()) {
                        if (setcate.second != 0) kv[setcate.first] = setcate.second;
                    }
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal && _key_sep != "") {
                    std::vector<std::string> key_splits = absl::StrSplit(attr.str_val(), _key_sep);
                    for (auto &ksp: key_splits) {
                        if (_val_sep == "") {
                            kv[ksp] = 1.0f;
                        } else {
                            std::vector<std::string> val_splits = absl::StrSplit(ksp, _val_sep);
                            float val = std::stof(val_splits[1]);
                            if (val != 0) kv[val_splits[0]] = val;
                        }
                    }
                } else {
                    _logger->warn(
                            "Set-categorical attribute (in_aname={}, out_gname={}) got neither map value nor string value but {}",
                            _in_aname, _out_gname, attr.oneof_attr_val_case());
                    return false;
                }

                if (kv.size() > 0) {
                    auto pred = [&](const char &ch) {
                        return algocomp::__is_forbidden_char<SetCateAttrOpCtx>(__setcate_attr_op_ctx, ch);
                    };

                    auto gen_feats = [&](const map<string, float> &kv) {
                        ContFeatGrp *cont_feat_grp = nullptr;
                        DiscFeatGrp *disc_feat_grp = nullptr;

                        if (_norm_type == _NORM_TYPE::USE_ONE) {
                            disc_feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                        } else {
                            cont_feat_grp = google::protobuf::Arena::CreateMessage<ContFeatGrp>(&arena);
                        }

                        for (auto it = kv.begin(); it != kv.end(); ++it) {
                            const string name_ = it->first;
                            float val = it->second;
                            string name;
                            if (name_.length() > _max_len) {
                                if (_exceed_rule == _EXCEED_RULE::SKIP) {
                                    _logger->debug(
                                            "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be discarded.",
                                            _in_aname, _out_gname, name_);
                                    return false;
                                } else if (_exceed_rule == _EXCEED_RULE::CUT_HEAD) {
                                    _logger->debug(
                                            "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be cut from head.",
                                            _in_aname, _out_gname, name_);
                                    name = name_.substr(0, _max_len);
                                } else if (_exceed_rule == _EXCEED_RULE::CUT_TAIL) {
                                    _logger->debug(
                                            "Length of fname (in_aname={}, out_gname={}, fval={}) exceeds max length, will be cut from tail.",
                                            _in_aname, _out_gname, name_);
                                    name = name_.substr(name_.length() - _max_len);
                                }
                            } else name = name_;

                            std::replace_if(name.begin(), name.end(), pred, '-');
                            string out_fname = absl::StrCat(_out_gname, "%", name);

                            if (_norm_type == _NORM_TYPE::USE_ONE && disc_feat_grp != nullptr) {
                                DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                                feat->set_gname(_out_gname);
                                feat->set_fname(out_fname);
                                (*disc_feat_grp->mutable_feats())[out_fname] = *feat;
                            } else if (cont_feat_grp != nullptr) {
                                ContFeatData *feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                                feat->set_gname(_out_gname);
                                feat->set_fname(out_fname);
                                feat->set_fval(val);
                                (*cont_feat_grp->mutable_feats())[out_fname] = *feat;
                            }
                        }

                        if (cont_feat_grp != nullptr) {
                            (*outp_feats.mutable_cont_feats())[_out_gname] = *cont_feat_grp;
                        }
                        else if (disc_feat_grp != nullptr) {
                            (*outp_feats.mutable_disc_feats())[_out_gname] = *disc_feat_grp;
                        }
                    };

                    if (_norm_type == _NORM_TYPE::NORM_SUM) {
                        float norm_sum = 0;
                        for (auto it = kv.begin(); it != kv.end(); ++it) norm_sum += it->second;
                        if (norm_sum != 0) {
                            map<string, float> kv_;
                            for (auto it = kv.begin(); it != kv.end(); ++it) kv_[it->first] = it->second / norm_sum;
                            gen_feats(kv_);
                        }
                    } else {
                        for (auto it = kv.begin(); it != kv.end(); ++it) gen_feats(kv);
                    }
                }

            }
            return true;

        } catch (const exception &ex) {
            _logger->error("Operated on wrong set-categorical attribute data: {}.", ex.what());
        }
        return false;
    }


}

