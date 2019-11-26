//
// Created by qfeng on 2019-01-08.
//

#include "attribute/attr_op_ctx.h"

namespace algocomp {
    bool AttrOpCtx::parse_patterns(const string in_aname, const string out_gname, const string out_fname,
                                   const string proc_desc, const string param_desc) {
        // Check the lengths of inputs (necessary inputs must not be empty)
        if (in_aname.length() == 0 || out_gname.length() == 0 || out_fname.length() == 0) {
            _logger->error("Parsed empty attribute patterns (in_aname = {}, out_gname = {}, out_fname = {}).", in_aname,
                           out_gname, out_fname);
            return false;
        }

        // Assign input attribute name
        _in_aname = in_aname;

        // Parse output feature group name
        if (out_gname[0] == '=') {
            _out_gname = _in_aname;
        } else {
            _out_gname = out_gname;
        }

        // Parse generation pattern of output feature name
        if (out_fname[0] == '=') {
            _gen_out_fname = _GEN_OUT_FNAME::EQ_OUT_GNAME;
        } else if (out_fname[0] == '%') {
            _gen_out_fname = _GEN_OUT_FNAME::EX_OUT_GNAME_WITH_VAL;
        } else {
            _logger->error("Parsed unknown out_fname {}", out_fname);
        }

        // Parse processes together with their parameters
        if (!parse_procs_and_params(proc_desc, param_desc)) {
            _logger->error("Parsed wrong procs and params (proc_desc={}, param_desc={}).", proc_desc, param_desc);
            return false;
        }

        // Check if the necessary information is parsed
        if (_gen_out_fname == _GEN_OUT_FNAME::UNKNOWN) {
            _logger->error("Parsed unknown out_fname generation configuration.");
            return false;
        }

        // Everything is OK
        return true;

    }

}
