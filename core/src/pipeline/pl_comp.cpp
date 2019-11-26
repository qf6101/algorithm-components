//
// Created by qfeng on 2019-01-26.
//

#include "pipeline/pl_comp.h"
#include "ctpl.h"

namespace algocomp {

    PLComp::PLComp(shared_ptr<ctpl::thread_pool> thp_rsrc, const ThreadType thread_type) :
    _thp_rsrc(thp_rsrc), _thread_type(thread_type) {
        _thread_type_str = thread_type_str(_thread_type);
    }


}


