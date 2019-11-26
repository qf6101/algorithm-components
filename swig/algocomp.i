%module algocomp_swig

%include "std_pair.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include various.i
%apply char *BYTE { char *attrs_pb };
%apply char *BYTE { char *feats_pb };
%apply char *BYTE { char *encs_pb };

%template(FloatVec) std::vector<float>;
%template(StrVec) std::vector<std::string>;

%ignore _attr_transer;
%ignore _feat_transer;
%ignore PLHead::PLHead;
%ignore PLHead::work(const char *proto_data, int len_proto_data, google::protobuf::Arena &arena);

%{
#include "common/type_def.h"
#include "log/algocomp_log.h"
#include "pipeline/pl_comp.h"
#include "pipeline/pl_head.h"
#include "pipeline/pl_middle.h"
#include "pipeline/pl_tail.h"
#include "pipeline/pipeline.h"
#include "pipeline/pl_comp_pool.h"
%}

%include "common/type_def.h"
%include "log/algocomp_log.h"
%include "pipeline/pl_comp.h"
%include "pipeline/pl_head.h"
%include "pipeline/pl_middle.h"
%include "pipeline/pl_tail.h"
%include "pipeline/pipeline.h"
%include "pipeline/pl_comp_pool.h"

