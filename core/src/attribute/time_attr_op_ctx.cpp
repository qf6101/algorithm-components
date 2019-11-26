//
// Created by qfeng on 2019-01-15.
//

#include "attribute/time_attr_op_ctx.h"
#include "log/algocomp_log.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include "absl/strings/str_cat.h"
#include <absl/strings/str_split.h>
#include <boost/algorithm/string.hpp>
#include <ctime>

using namespace boost::algorithm;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace std;


namespace algocomp {
    typedef boost::date_time::c_local_adjustor<ptime> local_adj;
    typedef boost::date_time::local_adjustor<ptime, +8, us_dst> cn_adj;

    const unsigned long TimeAttrOpCtx::TIME_LIMIT = 4102416000; //utc of 2100/01/01 (secs)

    TimeAttrOpCtx::TimeAttrOpCtx() {
        // Define Spdlog logger
        _logger = spdlog::get(ACLog::_logger_name);
    }

    bool TimeAttrOpCtx::parse_procs_and_params(const string &proc_desc, const string &param_desc) {
        try {
            // Check the lengths of inputs (necessary inputs must not be empty)
            if (proc_desc == "") {
                _logger->error("Empty procs for time attribute operation context {}.", _in_aname);
                return false;
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

                if (proc_name == "all") { // Transform with all time level
                    _hour_level = true;
                    _day_level = true;
                    _month_level = true;
                    _week_level = true;
                } else if (proc_name == "partial") { // Transform with partial time level (e.g., without hour level)
                    std::vector<std::string> time_params = absl::StrSplit(param_splits[i], ',');
                    for (int i = 0; i < time_params.size(); ++i) {
                        auto param_name = trim_copy(time_params[i]);

                        if (param_name == "hour") {
                            _hour_level = true;
                        } else if (param_name == "day") {
                            _day_level = true;
                        } else if (param_name == "month") {
                            _month_level = true;
                        } else if (param_name == "week") {
                            _week_level = true;
                        } else {
                            _logger->error("Parsed unknown time parameter {} (in_aname = {})", param_desc, _in_aname);
                            return false;
                        }
                    }
                } else if (proc_name == "time_str") { // Time string format if input data type is string
                    auto param_name = trim_copy(param_splits[i]);
                    _time_loc = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet(param_name));
                } else {
                    _logger->error("Parsed unknown time process {} (in_aname = {})", proc_desc, _in_aname);
                    return false;
                }
            }

            return true;
        } catch (const exception &ex) {
            _logger->error(
                    "Parsed wrong procs and params (proc_desc={}, param_desc={}) for time attribute (in_aname = {}): {}.",
                    proc_desc, param_desc, _in_aname, ex.what());
        }

        return false;
    }

    bool TimeAttrOpCtx::apply(const algocomp::AttributeBunch &inp_attrs, algocomp::FeatureBunch &outp_feats,
                              google::protobuf::Arena &arena) {
        try {
            // Match input attribute
            auto found = inp_attrs.attrs().find(_in_aname);
            if (found != inp_attrs.attrs().end()) {
                auto attr = (*found).second;

                // Parse attribute value with different data type
                long aval = 0;
                if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kIntVal) {
                    aval = attr.int_val();
                } else if (attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kLongVal) {
                    aval = attr.long_val();
                } else if(attr.oneof_attr_val_case() == AttributeData::OneofAttrValCase::kStrVal) {
                    auto time_str = trim_copy(attr.str_val());

                    if (time_str.length() == 0) {
                        _logger->debug("Empty string for time format (in_aname={}).", _in_aname);
                        return false;
                    }

                    std::istringstream is(time_str);
                    is.imbue(_time_loc);
                    boost::posix_time::ptime t;
                    is >> t;
                    t = cn_adj::local_to_utc(t);
                    auto base_time = boost::posix_time::from_time_t(0);
                    aval = (t - base_time).total_seconds();
                } else {
                    _logger->warn(
                            "Time attribute (in_aname={}, out_gname={}) got neither int value nor long value but {}",
                            _in_aname, _out_gname, attr.oneof_attr_val_case());
                    return false;
                }

                // millisecond to second
                if (aval / 1000000000 > 10) {
                    aval = aval / 1000;
                }

                if (aval > 0 && aval < TimeAttrOpCtx::TIME_LIMIT) {
                    // initialization
                    ptime t = from_time_t(aval);
                    t = local_adj::utc_to_local(t);
                    auto date = t.date();

                    if (_week_level) { // Create week level features
                        // weekday
                        int day_of_week = date.day_of_week();
                        if (day_of_week == 0) day_of_week = 7;
                        {
                            string out_gname = absl::StrCat(_out_gname, "+dayofweek");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(day_of_week));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }

                        // workday
                        int workday = 0;
                        if (day_of_week < 6) workday = 1;
                        {
                            string out_gname = absl::StrCat(_out_gname, "+workday");
                            string out_fname = out_gname;

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }
                    }

                    if (_hour_level) { // Create hour level features
                        // hour
                        int hour = t.time_of_day().hours();
                        {
                            string out_gname = absl::StrCat(_out_gname, "+hour");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(hour));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }

                        // hour description
                        int hour_desc = 0;
                        if (hour <= 6)
                            hour_desc = 1; // midnight
                        else if (hour <= 8)
                            hour_desc = 2; // early morning
                        else if (hour <= 11)
                            hour_desc = 3; // morning
                        else if (hour <= 13)
                            hour_desc = 4; // midnoon
                        else if (hour <= 18)
                            hour_desc = 5; //afternoon
                        else if (hour <= 21)
                            hour_desc = 6; //night
                        else
                            hour_desc = 7; //deep night
                        {
                            string out_gname = absl::StrCat(_out_gname, "+hourdesc");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(hour_desc));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }
                    }

                    if (_day_level) { // Create day level features
                        // day of month
                        auto day = date.day();
                        {
                            string out_gname = absl::StrCat(_out_gname, "+day");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(day));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }

                        // day description
                        int day_desc = 0;
                        if (day <= 10)
                            day_desc = 1;
                        else if (day <= 20)
                            day_desc = 2;
                        else
                            day_desc = 3;
                        {
                            string out_gname = absl::StrCat(_out_gname, "+daydesc");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(day_desc));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }
                    }

                    if (_month_level) { // Create month level features
                        // month
                        auto month = date.month();
                        {
                            string out_gname = absl::StrCat(_out_gname, "+month");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(month));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }

                        // year
                        auto year = date.year();
                        {
                            string out_gname = absl::StrCat(_out_gname, "+year");
                            string out_fname = absl::StrCat(out_gname, "%", std::to_string(year));

                            DiscFeatGrp *feat_grp = google::protobuf::Arena::CreateMessage<DiscFeatGrp>(&arena);
                            DiscFeatData *feat = google::protobuf::Arena::CreateMessage<DiscFeatData>(&arena);
                            feat->set_gname(out_gname);
                            feat->set_fname(out_fname);
                            (*feat_grp->mutable_feats())[out_fname] = *feat;
                            (*outp_feats.mutable_disc_feats())[out_gname] = *feat_grp;
                        }
                    }

                } else {
                    _logger->warn("Time value not in range ({}).", aval);
                    return false;
                }

            }

            // Everything is OK
            return true;

        } catch (const exception &ex) {
            _logger->error("Operated on wrong time attribute data (in_aname={}): {}.", _in_aname, ex.what());
        }

        return false;
    }
}
