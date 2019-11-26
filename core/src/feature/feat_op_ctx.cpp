//
// Created by qfeng on 2019-01-04.
//

#include "feature/feat_op_ctx.h"
#include <absl/strings/str_cat.h>
#include "log/algocomp_log.h"


namespace algocomp {

    const pair<string, string>
    UnaryOpCtx::out_names(const string &in_gname, const string &in_fname, const int in_fval) const {
        string out_gname, out_fname = "";

        // Generate output feature group name according to pattern
        if (_gen_out_gname == _GEN_OUT_GNAME::EX_IN_FNAME_WITH_OP) {
            out_gname = absl::StrCat(in_fname, "#", _op_name);
        }

        // Generate output feature name according to pattern
        if (_gen_out_fname == _GEN_OUT_FNAME::EQ_OUT_GNAME) {
            out_fname = out_gname;
        } else if (_gen_out_fname == _GEN_OUT_FNAME::EX_OUT_GNAME_WITH_VAL) {
            out_fname = absl::StrCat(out_gname, "@", std::to_string(in_fval));
        }

        return make_pair(out_gname, out_fname);
    }

    const pair<string, string> BinaryOpCtx::out_names(const string &in_gname1, const string &in_fname1,
                                                      const string &in_gname2, const string &in_fname2,
                                                      const string &suffix, const bool double_binary,
                                                      const int in_fval) const {
        string out_gname = "";
        string out_fname = "";

        string symbol = "^";

        // The input features are already the result of binary operation
        if (double_binary) symbol = "^^";

        // Generate output feature group name according to pattern
        if (_gen_out_gname == _GEN_OUT_GNAME::CONCAT_IN_GNAMES) {
            out_gname = absl::StrCat("!", _op_name, symbol, in_gname1, symbol, in_gname2);
        }

        // Generate output feature name according to pattern
        if (_gen_out_fname == _GEN_OUT_FNAME::EQ_OUT_GNAME) {
            out_fname = out_gname;
        } else if (_gen_out_fname == _GEN_OUT_FNAME::CONCAT_IN_FNAMES) {
            if (suffix.length() > 0) {
                // Append suffix to feature name
                out_fname = absl::StrCat("!", _op_name, "$", suffix, symbol, in_fname1, symbol, in_fname2);
            } else {
                out_fname = absl::StrCat("!", _op_name, symbol, in_fname1, symbol, in_fname2);
            }

        }

        return make_pair(out_gname, out_fname);
    }

    const bool UnaryOpCtx::parse_patterns(const string in_gname, const string in_fname, const string out_gname,
                                          const string out_fname, const string op_scope, const string proc_desc,
                                          const string param_desc) {

        // Check the lengths of inputs (necessary inputs must not be empty)
        if (in_gname.length() == 0 || in_fname.length() == 0 || out_gname.length() == 0 || out_fname.length() == 0 ||
            op_scope.length() == 0) {
            _logger->error(
                    "Parsed empty feature patterns (in_gname={}, in_fname={}, out_gname={}, out_fname={}, op_scope={}, proc_desc={}, param_desc={}).",
                    in_gname, in_fname, out_gname, out_fname, op_scope, proc_desc, param_desc);
            return false;
        }

        // Assign input feature group name
        _in_gname = in_gname;

        // Parse input feature name
        if (in_fname[0] == '=') {
            _in_fname = _in_gname;
        } else if (in_fname[0] != '%') {
            _in_fname = in_fname;
        }

        // Parse generation pattern of output feature group name
        if (out_gname[0] == '#') {
            _gen_out_gname = _GEN_OUT_GNAME::EX_IN_FNAME_WITH_OP;
        } else {
            _logger->error("Parsed unknown out_gname {}", out_gname);
        }

        // Parse generation pattern of output feature name
        if (out_fname[0] == '=') {
            _gen_out_fname = _GEN_OUT_FNAME::EQ_OUT_GNAME;
        } else if (out_fname[0] == '@') {
            _gen_out_fname = _GEN_OUT_FNAME::EX_OUT_GNAME_WITH_VAL;
        } else {
            _logger->error("Parsed unknown out_fname rule {}", out_fname);
        }

        // Parse operation scope
        if (op_scope[0] == 'g') {
            _op_scope = OP_SCOPE::GNAME;
        } else if (op_scope[0] == 'f') {
            _op_scope = OP_SCOPE::FNAME;
        } else {
            _logger->error("Parsed unknown op_scope {}", op_scope);
        }


        // Parse processes together with their parameters
        if (!parse_procs_and_params(proc_desc, param_desc)) {
            _logger->error("Parsed wrong procs or params (proc_desc={}, param_desc={}).", proc_desc, param_desc);
            return false;
        }

        // Check if the necessary information is parsed
        if (_op_scope == OP_SCOPE::UNKNOWN || _gen_out_gname == _GEN_OUT_GNAME::UNKNOWN ||
            _gen_out_fname == _GEN_OUT_FNAME::UNKNOWN) {
            _logger->error("Parsed unknown procs {} or params {}", proc_desc, param_desc);
            return false;
        } else if (_op_scope == OP_SCOPE::FNAME && _in_fname == "") {
            _logger->error("Parsed unmatched scope {} and fname {}", op_scope, in_fname);
            return false;
        } else return true; // Everything is OK
    }

    const bool BinaryOpCtx::parse_patterns(const string in_gname1, const string in_fname1, const string in_gname2,
                                           const string in_fname2, const string out_gname, const string out_fname,
                                           const string op_scope, const string proc_desc, const string param_desc) {

        // Check the lengths of inputs (necessary inputs must not be empty)
        if (in_gname1.length() == 0 || in_fname1.length() == 0 || in_gname2.length() == 0 || in_fname2.length() == 0 ||
            out_gname.length() == 0 || out_fname.length() == 0 || op_scope.length() == 0) {
            _logger->error(
                    "Parsed empty feature patterns (in_gname1={}, in_fname1={}, in_gname2={}, in_fname2={}, out_gname={}, out_fname={}, op_scope={}, proc_desc={}, param_desc={}).",
                    in_gname1, in_fname1, in_gname2, in_fname2, out_gname, out_fname, op_scope, proc_desc, param_desc);
            return false;
        }

        // Assign input feature group names
        _in_gname1 = in_gname1;
        _in_gname2 = in_gname2;

        // Parse input feature names
        if (in_fname1[0] == '=') {
            _in_fname1 = _in_gname1;
        } else if (in_fname1[0] != '%') {
            _in_fname1 = in_fname1;
        }

        if (in_fname2[0] == '=') {
            _in_fname2 = _in_gname2;
        } else if (in_fname2[0] != '%') {
            _in_fname2 = in_fname2;
        }

        // Parse generation pattern of output feature group name
        if (out_gname[0] == '!') {
            _gen_out_gname = _GEN_OUT_GNAME::CONCAT_IN_GNAMES;
        } else {
            _logger->error("Parsed unknown out_gname {}", out_gname);
        }

        // Parse generation pattern of output feature name
        if (out_fname[0] == '=') {
            _gen_out_fname = _GEN_OUT_FNAME::EQ_OUT_GNAME;
        } else if (out_fname[0] == '!') {
            _gen_out_fname = _GEN_OUT_FNAME::CONCAT_IN_FNAMES;
        } else {
            _logger->error("Parsed unknown out_fname rule {}", out_fname);
        }

        // Parse operation scope
        if (op_scope[0] == 'g') {
            _op_scope = OP_SCOPE::GNAME;
        } else if (op_scope[0] == 'f') {
            _op_scope = OP_SCOPE::FNAME;
        } else {
            _logger->error("Parsed unknown op_scope {}", op_scope);
        }

        // Parse processes together with their parameters
        if (!parse_procs_and_params(proc_desc, param_desc)) {
            _logger->error("Parsed wrong procs or params (proc_desc={}, param_desc={}).", proc_desc, param_desc);
            return false;
        }

        // Check if the necessary information is parsed
        if (_op_scope == OP_SCOPE::UNKNOWN || _gen_out_gname == _GEN_OUT_GNAME::UNKNOWN ||
            _gen_out_fname == _GEN_OUT_FNAME::UNKNOWN) {
            _logger->error("Parsed unknown procs {} or params {}", proc_desc, param_desc);
            return false;
        } else if (_op_scope == OP_SCOPE::FNAME && (_in_fname1 == "" || _in_fname2 == "")) {
            _logger->error("Parsed unmatched scope {}, fname1 {}, fname2 {}", op_scope, proc_desc, param_desc);
        } else return true; // Everything is OK
    }

}