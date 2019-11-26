//
// Created by qfeng on 2019-01-08.
//

#include "attribute/rv_attr_op_ctx.h"
#include "absl/strings/str_cat.h"
#include <absl/strings/str_split.h>
#include "log/algocomp_log.h"
#include <boost/algorithm/string.hpp>

using namespace boost::algorithm;

namespace algocomp {
    RVAttrOpCtx::RVAttrOpCtx() {
        // Define Spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
    }

    bool RVAttrOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "" && param_desc == "") {
                _logger->warn("Empty procs and params for real valued attribute operation context (in_aname={}).",
                              _in_aname);
                return true;
            }

            // Parse processes together with their parameters
            std::vector<std::string> proc_splits = absl::StrSplit(proc_desc, ';');
            std::vector<std::string> param_splits = absl::StrSplit(param_desc, ';');

            // The processes and parameters are corresponding one by one. The belows are to check the correctness
            if (proc_splits.size() != param_splits.size()) {
                _logger->error("Parsed inconsistent sizes of procs {} params {} (in_aname={})", proc_desc, param_desc,
                               _in_aname);
                return false;
            }

            for (int i = 0; i < proc_splits.size(); ++i) {
                auto proc_name = trim_copy(proc_splits[i]);

                if (proc_name == "prune_range") { // Parse process of over range prune
                    std::vector<std::string> ranges = absl::StrSplit(param_splits[i], ',');
                    // Parse parameter of over range prune
                    if (trim_copy(ranges[0]) == "skip") {
                        _over_range_rule = _OVER_RANGE_RULE::SKIP_DATA;
                    } else if (trim_copy(ranges[0]) == "replace") {
                        _over_range_rule = _OVER_RANGE_RULE::REPLACE_WITH_MIN_OR_MAX;
                    } else {
                        _logger->error("Configured wrong prune range rule {} (in_aname={}).", ranges[0], _in_aname);
                        return false;
                    }

                    // Parse parameter of minimum and maximum values
                    _range_min = std::stof(ranges[1]);
                    _range_max = std::stof(ranges[2]);
                } else if (proc_name == "log") { // Parse process of log computation
                    std::vector<std::string> log_params = absl::StrSplit(param_splits[i], ',');

                    // Parse parameter of logarithm type
                    if (trim_copy(log_params[0]) == "log10") {
                        _log_type = _LogType::LOG10;
                    } else if (trim_copy(log_params[0]) == "log") {
                        _log_type = _LogType::LOG;
                    } else if (trim_copy(log_params[0]) == "log2") {
                        _log_type = _LogType::LOG2;
                    } else {
                        _logger->error("Parsed unknown log type param {} (in_aname={})", param_splits[i], _in_aname);
                        return false;
                    }

                    // Parse parameter of minimum negative value (optional)
                    // logarithm computation: x = x - _min_neg_value, followed by x = log(x)
                    if (log_params.size() > 1) {
                        float min_neg_val = std::stof(trim_copy(log_params[1]));
                        if (min_neg_val >= 0) {
                            _logger->error("Parsed positive or zero min_neg_val {} (in_aname={}).", _min_neg_val,
                                           _in_aname);
                        } else {
                            _min_neg_val = min_neg_val;
                        }
                    }
                } else if (proc_name == "scale_min_max") { // Parse process of min-max scale
                    _scale_type = _SCALE_TYPE::MIN_MAX;
                    std::vector<std::string> min_and_max = absl::StrSplit(param_splits[i], ',');
                    _scale_param1 = std::stof(min_and_max[0]);
                    _scale_param2 = std::stof(min_and_max[1]);
                } else if (proc_name == "scale_z_score") { // Parse process of standard scale
                    _scale_type = _SCALE_TYPE::Z_SCORE;
                    std::vector<std::string> mean_and_std = absl::StrSplit(param_splits[i], ',');
                    _scale_param1 = std::stof(mean_and_std[0]);
                    _scale_param2 = std::stof(mean_and_std[1]);
                }
            }

            return true;

        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong procs and params (proc_desc={}, param_desc={}) for real valued attribute (in_aname={}): {}.",
                    proc_desc, param_desc, _in_aname, ex.what());
        }

        return false;
    }

    bool RVAttrOpCtx::apply(const algocomp::AttributeBunch &inp_attrs, algocomp::FeatureBunch &outp_feats,
                            google::protobuf::Arena &arena) {
        try {
            // Match input attribute
            auto found = inp_attrs.attrs().find(_in_aname);
            if (found != inp_attrs.attrs().end()) {
                auto attr = (*found).second;

                // Parse attribute value with different data type
                float aval = 0;
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kIntVal) {
                    aval = attr.int_val();
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kFloatVal) {
                    aval = attr.float_val();
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal) {
                    try {
                        aval = std::stof(trim_copy(attr.str_val()));
                    } catch (const exception &ex) {
                        _logger->error("Parsed invalid str value {} for real valued attribute (in_aname={}): {}",
                                       attr.str_val(), _in_aname, ex.what());
                    }
                } else {
                    _logger->warn(
                            "Realvalued attribute (in_aname={}, out_gname={}) got neither int value nor float value but {}",
                            _in_aname, _out_gname, attr.oneof_attr_val_case());
                    return false;
                }

                if (aval != 0) {
                    // Treat over range value
                    if (_over_range_rule == _OVER_RANGE_RULE::SKIP_DATA) {
                        if (aval < _range_min || aval > _range_max) {
                            _logger->debug(
                                    "Attribute value (in_aname={}, out_gname={}) exceeds range which will be discarded.",
                                    _in_aname, _out_gname);
                            return false;
                        }
                    } else if (_over_range_rule == _OVER_RANGE_RULE::REPLACE_WITH_MIN_OR_MAX) {
                        if (aval < _range_min) {
                            _logger->debug(
                                    "Attribute value (in_aname={}, out_gname={}) exceeds range which will be replaced with min value {}.",
                                    _in_aname, _out_gname, std::to_string(_range_min));
                            aval = _range_min;
                        } else if (aval > _range_max) {
                            _logger->debug(
                                    "Attribute value (in_aname={}, out_gname={}) exceeds range which will be replaced with max value {}.",
                                    _in_aname, _out_gname, std::to_string(_range_max));
                            aval = _range_max;
                        }
                    }

                    // Treat logarithm computation
                    if (_log_type != _LogType::NO && aval < 0) {
                        aval = aval + 1 - _min_neg_val;

                        if (aval <= 0) {
                            _logger->warn("Get minus number or zero after translating ({}).", aval);
                            return false;
                        }
                    }

                    if (_log_type == _LogType::LOG10) {
                        aval = log10(aval);
                    } else if (_log_type == _LogType::LOG2) {
                        aval = log2(aval);
                    } else if (_log_type == _LogType::LOG) {
                        aval = log(aval);
                    }

                    // Treat value scale
                    if (_scale_type == _SCALE_TYPE::MIN_MAX) {
                        aval = (aval - _scale_param1) / (_scale_param2 - _scale_param1);
                    } else if (_scale_type == _SCALE_TYPE::Z_SCORE) {
                        aval = (aval - _scale_param1) / _scale_param2;

                        if (_after_zscore == _AFTER_ZSCORE::GAUSSIAN_TO_UNIFORM) {
                            aval = std::erfc(-aval / 1.414) * 0.5;
                        }
                    }

                    // Create output feature
                    if (aval == 0) aval = 0.0001;
                    ContFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<ContFeatGrp>(&arena);
                    ContFeatData *feat = google::protobuf::Arena::CreateMessage<ContFeatData>(&arena);
                    feat->set_gname(_out_gname);
                    feat->set_fname(_out_gname);
                    feat->set_fval(aval);
                    (*feat_grp->mutable_feats())[_out_gname] = *feat;
                    (*outp_feats.mutable_cont_feats())[_out_gname] = *feat_grp;
                } else {
                    _logger->debug("Attribute value (in_aname={}, out_gname={}) is zero which maybe not set properly.",
                                   _in_aname, _out_gname);
                    return false;
                }
            }
            // Everything is OK
            return true;

        } catch (const exception &ex) {
            _logger->error("Operated on wrong real valued attribute data: {}.", ex.what());
        }
        return false;
    }

}