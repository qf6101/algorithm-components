//
// Created by qfeng on 2019-01-24.
//

#include "common/type_def.h"

namespace algocomp {

    const string thread_type_str(const ThreadType &tt) {
        if (tt == ThreadType::NO) {
            return "no";
        } else if (tt == ThreadType::ON_ADS) {
            return "on-ads";
        } else {
            return "on-example";
        }
    }

}
